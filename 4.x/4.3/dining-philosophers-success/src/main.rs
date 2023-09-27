use rand::Rng;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

const PHILOSOPHERS_COUNT: usize = 5;

fn philosopher(id: usize, left_chopstick: Arc<Mutex<()>>, right_chopstick: Arc<Mutex<()>>) {
    let mut rng = rand::thread_rng();
    loop {
        println!("Philosopher {} is thinking", id);

        if id == PHILOSOPHERS_COUNT - 1 {
            let _left_guard = left_chopstick.lock().unwrap();
            println!("Philosopher {} picked up the left chopstick.", id);

            let _right_guard = right_chopstick.lock().unwrap();
            println!("Philosopher {} picked up the right chopstick.", id);
        } else {
            let _right_guard = right_chopstick.lock().unwrap();
            println!("Philosopher {} picked up the right chopstick.", id);

            let _left_guard = left_chopstick.lock().unwrap();
            println!("Philosopher {} picked up the left chopstick.", id);
        }

        let eat_duration = rng.gen_range(0..10000);
        println!("Philosopher {} is eating.", id);
        thread::sleep(Duration::from_millis(eat_duration));

        println!("Philosopher {} dropped the right chopstick.", id);
        println!("Philosopher {} dropped the left chopstick.", id);
    }
}

fn main() {
    let chopsticks: Vec<_> = (0..PHILOSOPHERS_COUNT)
        .map(|_| Arc::new(Mutex::new(())))
        .collect();
    let mut handles = vec![];

    for i in 0..PHILOSOPHERS_COUNT {
        let left_chopstick = chopsticks[i].clone();
        let right_chopstick = chopsticks[(i + 1) % PHILOSOPHERS_COUNT].clone();

        handles.push(thread::spawn(move || {
            philosopher(i, left_chopstick, right_chopstick)
        }));
    }

    for handle in handles {
        handle.join().unwrap();
    }
}
