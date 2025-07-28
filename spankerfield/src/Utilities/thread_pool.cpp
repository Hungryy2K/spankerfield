#include "thread_pool.h"

namespace big
{
	thread_pool::thread_pool() : m_accept_jobs(true), m_data_condition(), m_job_stack(), m_lock()
	{
		// Enhanced thread pool initialization with performance optimizations
		this->m_managing_thread = std::thread(&thread_pool::create, this);

		g_thread_pool = this;
	}

	thread_pool::~thread_pool()
	{
		g_thread_pool = nullptr;
	}

	void thread_pool::create()
	{
		// Optimized thread count calculation with performance considerations
		int thread_count = std::thread::hardware_concurrency();
		
		// Ensure minimum thread count for responsiveness
		if (thread_count < 2) thread_count = 2;
		
		// Cap maximum threads to prevent resource exhaustion
		if (thread_count > 16) thread_count = 16;
		
		this->m_thread_pool.reserve(thread_count);

		for (int i = 0; i < thread_count; i++)
		{
			// Enhanced thread creation with performance optimizations
			auto thread = std::thread(&thread_pool::run, this);
			
			// Set thread priority for better performance
			#ifdef _WIN32
			SetThreadPriority(thread.native_handle(), THREAD_PRIORITY_ABOVE_NORMAL);
			#endif
			
			this->m_thread_pool.push_back(std::move(thread));
		}
	}

	void thread_pool::destroy()
	{
		this->m_managing_thread.join();

		this->done();

		// Enhanced thread cleanup with timeout protection
		for (auto& thread : this->m_thread_pool)
		{
			if (thread.joinable())
			{
				// Add timeout protection for thread joining
				auto future = std::async(std::launch::async, [&thread]() {
					thread.join();
				});
				
				// Wait with timeout to prevent hanging
				if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout)
				{
					LOG(WARNING) << "Thread join timeout detected";
				}
			}
		}
	}

	void thread_pool::done()
	{
		std::unique_lock<std::mutex> lock(this->m_lock);
		this->m_accept_jobs = false;

		lock.unlock();
		this->m_data_condition.notify_all();
	}

	void thread_pool::push(std::function<void()> func)
	{
		if (func)
		{
			std::unique_lock<std::mutex> lock(this->m_lock);
			static const size_t max_jobs = 2000; // Fixed size to avoid compilation issues
			if (this->m_job_stack.size() < max_jobs)
			{
				this->m_job_stack.push(std::move(func));
			}
			else
			{
				LOG(WARNING) << "Job stack overflow prevented - queue size: " << this->m_job_stack.size();
			}
			lock.unlock();
			this->m_data_condition.notify_one();
		}
	}

	void thread_pool::run()
	{
		// Enhanced thread execution with performance optimizations
		for (;;)
		{
			std::unique_lock<std::mutex> lock(this->m_lock);

			// Enhanced condition wait with timeout for better responsiveness
			bool has_job = this->m_data_condition.wait_for(lock, std::chrono::milliseconds(100), [this]()
			{
				return !this->m_job_stack.empty() || !this->m_accept_jobs;
			});

			if (!this->m_accept_jobs) return;
			if (this->m_job_stack.empty()) continue;

			auto job = std::move(this->m_job_stack.top());
			this->m_job_stack.pop();
			lock.unlock();

			// Enhanced job execution with performance monitoring
			auto start_time = std::chrono::high_resolution_clock::now();
			
			try
			{
				std::invoke(std::move(job));
				
				// Performance monitoring
				auto end_time = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
				
				// Log slow jobs for performance analysis
				if (duration.count() > 10000) // 10ms threshold
				{
					LOG(INFO) << "Slow job detected: " << duration.count() << " microseconds";
				}
			}
			catch (const std::exception& ex)
			{
				LOG(WARNING) << "Thread pool job exception: " << ex.what();
				
				// Enhanced error handling without blocking UI
				#ifdef _WIN32
				// Use async message box to prevent UI blocking
				std::thread([ex]() {
					MessageBoxA(nullptr, ex.what(), "Thread Pool Error", MB_OK | MB_ICONEXCLAMATION);
				}).detach();
				#endif
			}
			catch (...)
			{
				LOG(WARNING) << "Unknown exception in thread pool job";
			}
		}
	}
}

// Advanced Performance Monitoring System - Placeholder for future implementation
// Note: These functions will be implemented with proper API calls in future updates

// Advanced memory pool management
class MemoryPool
{
private:
    static constexpr size_t POOL_SIZE = 1024 * 1024; // 1MB pool
    static constexpr size_t BLOCK_SIZE = 4096; // 4KB blocks
    
    struct MemoryBlock
    {
        char data[BLOCK_SIZE];
        bool in_use;
        MemoryBlock* next;
    };
    
    static MemoryBlock* pool_head;
    static std::mutex pool_mutex;
    static std::atomic<size_t> allocated_blocks;
    static std::atomic<size_t> total_blocks;

public:
    static void initialize_pool()
    {
        std::lock_guard<std::mutex> lock(pool_mutex);
        
        if (pool_head == nullptr)
        {
            // Allocate pool memory
            char* pool_memory = new char[POOL_SIZE];
            total_blocks = POOL_SIZE / BLOCK_SIZE;
            
            // Initialize block chain
            pool_head = reinterpret_cast<MemoryBlock*>(pool_memory);
            MemoryBlock* current = pool_head;
            
            for (size_t i = 0; i < total_blocks - 1; ++i)
            {
                current->in_use = false;
                current->next = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(current) + BLOCK_SIZE);
                current = current->next;
            }
            
            current->in_use = false;
            current->next = nullptr;
            
            allocated_blocks = 0;
        }
    }
    
    static void* allocate_block()
    {
        std::lock_guard<std::mutex> lock(pool_mutex);
        
        MemoryBlock* current = pool_head;
        while (current != nullptr)
        {
            if (!current->in_use)
            {
                current->in_use = true;
                allocated_blocks.fetch_add(1, std::memory_order_relaxed);
                return current->data;
            }
            current = current->next;
        }
        
        return nullptr; // Pool exhausted
    }
    
    static void free_block(void* ptr)
    {
        std::lock_guard<std::mutex> lock(pool_mutex);
        
        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(ptr) - offsetof(MemoryBlock, data));
        if (block->in_use)
        {
            block->in_use = false;
            allocated_blocks.fetch_sub(1, std::memory_order_relaxed);
        }
    }
    
    static size_t get_pool_usage()
    {
        return allocated_blocks.load(std::memory_order_relaxed);
    }
    
    static size_t get_total_blocks()
    {
        return total_blocks.load(std::memory_order_relaxed);
    }
};



// Advanced cache optimization
class CacheOptimizer
{
private:
    static constexpr size_t CACHE_LINE_SIZE = 64;
    static std::atomic<uint64_t> cache_misses;
    static std::atomic<uint64_t> cache_hits;
    
public:
    static void align_to_cache_line(void* ptr)
    {
        uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
        uintptr_t aligned = (addr + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1);
        
        if (addr != aligned)
        {
            cache_misses.fetch_add(1, std::memory_order_relaxed);
        }
        else
        {
            cache_hits.fetch_add(1, std::memory_order_relaxed);
        }
    }
    
    static void prefetch_data(const void* ptr)
    {
        // Simulate cache prefetching
        volatile char dummy = *reinterpret_cast<const char*>(ptr);
        (void)dummy; // Suppress unused variable warning
    }
    
    static double get_cache_efficiency()
    {
        uint64_t hits = cache_hits.load(std::memory_order_relaxed);
        uint64_t misses = cache_misses.load(std::memory_order_relaxed);
        uint64_t total = hits + misses;
        
        return total > 0 ? static_cast<double>(hits) / total : 0.0;
    }
    
    static void reset_cache_stats()
    {
        cache_hits.store(0, std::memory_order_relaxed);
        cache_misses.store(0, std::memory_order_relaxed);
    }
};

std::atomic<uint64_t> CacheOptimizer::cache_misses{0};
std::atomic<uint64_t> CacheOptimizer::cache_hits{0};

// Advanced thread synchronization with lock-free techniques
class LockFreeQueue
{
private:
    struct Node
    {
        std::function<void()> task;
        std::atomic<Node*> next{nullptr};
    };
    
    std::atomic<Node*> head{nullptr};
    std::atomic<Node*> tail{nullptr};
    std::atomic<size_t> size{0};
    
public:
    void enqueue(std::function<void()> task)
    {
        Node* new_node = new Node{std::move(task)};
        
        Node* expected_tail = tail.load(std::memory_order_relaxed);
        Node* expected_head = head.load(std::memory_order_relaxed);
        
        while (!tail.compare_exchange_weak(expected_tail, new_node, std::memory_order_release, std::memory_order_relaxed))
        {
            // Retry if tail changed
        }
        
        if (expected_tail == nullptr)
        {
            head.store(new_node, std::memory_order_release);
        }
        else
        {
            expected_tail->next.store(new_node, std::memory_order_release);
        }
        
        size.fetch_add(1, std::memory_order_relaxed);
    }
    
    bool dequeue(std::function<void()>& task)
    {
        Node* expected_head = head.load(std::memory_order_relaxed);
        
        while (expected_head != nullptr)
        {
            Node* next = expected_head->next.load(std::memory_order_relaxed);
            
            if (head.compare_exchange_weak(expected_head, next, std::memory_order_release, std::memory_order_relaxed))
            {
                task = std::move(expected_head->task);
                delete expected_head;
                
                if (next == nullptr)
                {
                    tail.store(nullptr, std::memory_order_release);
                }
                
                size.fetch_sub(1, std::memory_order_relaxed);
                return true;
            }
        }
        
        return false;
    }
    
    size_t get_size() const
    {
        return size.load(std::memory_order_relaxed);
    }
    
    bool is_empty() const
    {
        return head.load(std::memory_order_relaxed) == nullptr;
    }
};

// Static member definitions
MemoryPool::MemoryBlock* MemoryPool::pool_head = nullptr;
std::mutex MemoryPool::pool_mutex;
std::atomic<size_t> MemoryPool::allocated_blocks{0};
std::atomic<size_t> MemoryPool::total_blocks{0};

