use rand::Rng;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

const PHILOSOPHERS_COUNT: usize = 5;

fn philosopher(
    id: usize,
    left_chopstick: Arc<Mutex<()>>,
    right_chopstick: Arc<Mutex<()>>,
    right_chopstick_counter: Arc<Mutex<usize>>,
) {
    let mut rng = rand::thread_rng();
    loop {
        println!("Philosopher {} is thinking", id);
        {
            let _left_guard = left_chopstick.lock().unwrap();
            println!("Philosopher {} picked up the left chopstick.", id);

            {
                let mut counter = right_chopstick_counter.lock().unwrap();
                *counter += 1;
            }

            println!(
                "Philosopher {} is trying to pick up the right chopstick.",
                id
            );
            let _right_guard = right_chopstick.lock().unwrap();
            println!("Philosopher {} picked up the right chopstick.", id);

            {
                let mut counter = right_chopstick_counter.lock().unwrap();
                *counter -= 1;
            }

            let eat_duration = rng.gen_range(0..10000);
            println!("Philosopher {} is eating.", id);
            thread::sleep(Duration::from_millis(eat_duration));

            println!("Philosopher {} dropped the right chopstick.", id);
            println!("Philosopher {} dropped the left chopstick.", id);
        }
    }
}

fn deadlock_detector(counter: Arc<Mutex<usize>>) {
    loop {
        thread::sleep(Duration::from_millis(10));
        let c = {
            let count = counter.lock().unwrap();
            *count
        };
        if c == PHILOSOPHERS_COUNT {
            println!("DEADLOCK DETECTED!");
            break;
        }
    }
}

fn main() {
    let chopsticks: Vec<_> = (0..PHILOSOPHERS_COUNT)
        .map(|_| Arc::new(Mutex::new(())))
        .collect();
    let right_chopstick_counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for i in 0..PHILOSOPHERS_COUNT {
        let left_chopstick = chopsticks[i].clone();
        let right_chopstick = chopsticks[(i + 1) % PHILOSOPHERS_COUNT].clone();
        let counter_clone = right_chopstick_counter.clone();

        handles.push(thread::spawn(move || {
            philosopher(i, left_chopstick, right_chopstick, counter_clone)
        }));
    }

    let detector_handle = thread::spawn({
        let counter_clone = right_chopstick_counter.clone();
        move || deadlock_detector(counter_clone)
    });

    for handle in handles {
        handle.join().unwrap();
    }

    detector_handle.join().unwrap();
}
