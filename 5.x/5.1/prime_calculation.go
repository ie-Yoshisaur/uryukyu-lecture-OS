package main

import (
	"fmt"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
	"math"
	"sync"
	"time"
)

func isPrime(number int) bool {
	if number <= 1 {
		return false
	}
	for i := 2; i <= int(math.Sqrt(float64(number))); i++ {
		if number%i == 0 {
			return false
		}
	}
	return true
}

func worker(jobs <-chan int, results chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	for number := range jobs {
		if isPrime(number) {
			results <- number
		}
	}
}

func calculatePrimesConcurrent(workerCount int, start int, end int) ([]int, time.Duration) {
	jobs := make(chan int, end-start+1)
	results := make(chan int, end-start+1)

	var wg sync.WaitGroup

	for w := 1; w <= workerCount; w++ {
		wg.Add(1)
		go worker(jobs, results, &wg)
	}

	startTime := time.Now()
	for number := start; number <= end; number++ {
		jobs <- number
	}
	close(jobs)

	wg.Wait()
	close(results)

	primes := make([]int, 0)
	for prime := range results {
		primes = append(primes, prime)
	}

	return primes, time.Since(startTime)
}

func calculatePrimesSingleThread(start int, end int) ([]int, time.Duration) {
	startTime := time.Now()
	primes := make([]int, 0)
	for number := start; number <= end; number++ {
		if isPrime(number) {
			primes = append(primes, number)
		}
	}
	return primes, time.Since(startTime)
}

func main() {
	m := 10000000
	n := 20000000

	workerCounts := []int{1, 2, 3, 4}
	times := make([]time.Duration, len(workerCounts))

	for i, workerCount := range workerCounts {
		_, elapsedParallel := calculatePrimesConcurrent(workerCount, 3, m)
		_, elapsedSingle := calculatePrimesSingleThread(m+1, n)
		totalElapsed := elapsedParallel + elapsedSingle
		times[i] = totalElapsed
		fmt.Printf("Total time with %d worker(s): %s\n", workerCount, totalElapsed)
	}

	// Create a new plot for visualizing the results
	p := plot.New()

	p.Title.Text = "Performance comparison according to Amdahl's Law"
	p.X.Label.Text = "Number of Workers"
	p.Y.Label.Text = "Time (seconds)"

	points := make(plotter.XYs, len(workerCounts))

	for i, workerCount := range workerCounts {
		points[i].X = float64(workerCount)
		points[i].Y = times[i].Seconds()
	}

	line, pointsScatter, err := plotter.NewLinePoints(points)
	if err != nil {
		panic(err)
	}

	p.Add(line, pointsScatter)
	p.Legend.Add("Worker times", line, pointsScatter)

	// Save the plot to a SVG file.
	if err := p.Save(6*vg.Inch, 8*vg.Inch, "amdahl.svg"); err != nil {
		panic(err)
	}
}
