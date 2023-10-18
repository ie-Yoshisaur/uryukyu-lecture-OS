package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

// Semaphore is a type representing a semaphore.
type Semaphore chan struct{}

// NewSemaphore creates a new semaphore with the specified number of permits.
func NewSemaphore(n int) Semaphore {
	return make(Semaphore, n)
}

// Acquire obtains one permit from the semaphore.
func (s Semaphore) Acquire() {
	s <- struct{}{} // Send to the channel to acquire a permit. This will block if the channel is full.
}

// Release releases one permit back to the semaphore.
func (s Semaphore) Release() {
	<-s // Receive from the channel to release a permit.
}

func heavyProcess(id int, sema Semaphore, wg *sync.WaitGroup) {
	defer wg.Done() // Decrement the counter of the WaitGroup as this goroutine completes.

	sema.Acquire()       // Acquire a permit from the semaphore.
	defer sema.Release() // Release the permit when this function exits.

	// Simulate some heavy work.
	fmt.Printf("Goroutine %d is starting heavy processing\n", id)
	processTime := time.Duration(rand.Intn(3)+1) * time.Second
	time.Sleep(processTime) // Representing heavy work just by sleeping here.
	fmt.Printf("Goroutine %d has completed heavy processing in %v\n", id, processTime)
}

func main() {
	rand.Seed(time.Now().UnixNano()) // Initialize the random seed.
	sema := NewSemaphore(3)          // Only 3 goroutines are allowed to run concurrently.
	var wg sync.WaitGroup            // WaitGroup for waiting for all goroutines to complete.

	for i := 0; i < 10; i++ {
		wg.Add(1) // Increment the WaitGroup counter for each goroutine.
		go heavyProcess(i, sema, &wg)
	}

	wg.Wait() // Wait for all goroutines to complete.
	fmt.Println("All goroutines have completed processing")
}
