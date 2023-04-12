#pragma once 
#include <iostream>

#include <cassert>
#include <cstdlib>
#include <memory>

namespace  mr
{

//THIS IS A DANGER ZONE, DONT USE CURRENTLY.
//DIRECT SOURCE OF THE MOST BUGS; NOT FIGURABLE

// Well. I didnt want to reinvent the wheal but i didnt find an equivalent of it in STL
// As soon as I find a better implemention I will remove this.

//This is actualy inspired from V8 source code 
// Acts like a variable stack on heap but dynamic.
// Once you done your work you can free all of your structs in this zone by destructing it or calling destroy
// A typed zone is a std::vector use it wherever possible
struct Zone
{
	private:
	typedef  unsigned char byte;
	
	
	std::size_t capacity;
	std::size_t space;
	byte* buffer{};
	void* ptr{};
	
	void ensure_capcity(std::size_t req_size)
	{
		if(req_size>space){
			std::size_t offset=((byte*)ptr-buffer);
			std::size_t least_cap=offset+req_size;
			capacity= calc_newcap(capacity,least_cap);
			buffer=(byte*)std::realloc(buffer,capacity);
			assert(buffer);
			ptr=buffer+offset;
			space=capacity-offset;
		}
	}
	void expand()
	{
		std::size_t offset=((byte*)ptr-buffer);
		capacity*=2;
		buffer=(byte*)std::realloc(buffer,capacity);
		assert(buffer);
		ptr=buffer+offset;
		space=capacity-offset;
	}
	public: 
	Zone(std::size_t inital_cap=32):capacity{inital_cap}, space(capacity)
	{
		assert((capacity & (capacity - 1) && "should be power of 2") == 0);
		buffer=(byte*)std::malloc(capacity);
		ptr=buffer;
		if(!ptr) std::abort();
	}
	Zone(Zone&& other){
		buffer=other.buffer;
		other.buffer=nullptr;
		ptr=other.ptr;
		other.ptr=nullptr;
		capacity=other.capacity;
		other.capacity=0;
		space=other.space;
		other.space=0;
	}
	void operator =(Zone&& other)
	{
		destroy();
		buffer=other.buffer;
		other.buffer=nullptr;
		ptr=other.ptr;
		other.ptr=nullptr;
		capacity=other.capacity;
		other.capacity=0;
		space=other.space;
		other.space=0;
	}
	Zone(const Zone&)=delete;
	void operator=(const Zone&)=delete;
	std::size_t calc_newcap(std::size_t old_cap,std::size_t least_cap){
		std::size_t new_cap=old_cap;
		while(true){
			if(least_cap<=new_cap) return new_cap;
			else new_cap*=2;
		}
	}
	
	// allocates storage for n instances of T
	template <typename T>
    T* allocate(std::size_t n)
    {
        
        //TODO: Does compiler understand  we have aligned pointer?
        std::size_t array_size=n* sizeof(T);
        ensure_capcity(array_size);
        std::size_t alignment = alignof(T);
        if (std::align(alignment, array_size, ptr ,space))
        {
            
            T* result = reinterpret_cast<T*>(ptr);
            ptr = (byte*)ptr + array_size;
            
            space -= array_size;
            /*std::cout<<"allocated "<<n<<" of aligment "<<alignment<<"\n"<<
         	"address: "<<(void*)((byte*) ptr-array_size)<<" \n"
         	"capacity: "<< capacity <<"\n"
         	"space: "<<space<<"\n"
         	"buffer: "<<(void*) buffer<<std::endl<<std::endl;*/
            return result;
        }
        //request more space to fit alligment
        expand();
       return allocate<T>(n);
    }
    void destroy(){
    	std::free(buffer);
    }
    ~Zone(){
    	destroy();
    }
};



}