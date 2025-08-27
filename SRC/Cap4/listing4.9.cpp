#include <pthread.h>
#include <iostream>

class ThreadExitException
{
public:
    explicit ThreadExitException(void* return_value)
        : thread_return_value_(return_value) {}

    void* DoThreadExit()
    {
        pthread_exit(thread_return_value_);
    }

private:
    void* thread_return_value_;
};

// Prototipo de la función que determina si el thread debe salir
bool should_exit_thread_immediately();

void do_some_work()
{
    while (true) {
        // Hacer algo útil aquí...
        if (should_exit_thread_immediately())
            throw ThreadExitException(nullptr); // nullptr en C++
    }
}

void* thread_function(void*)
{
    try {
        do_some_work();
    }
    catch (ThreadExitException& ex) { // Captura por referencia
        ex.DoThreadExit();
    }
    return nullptr;
}

// Ejemplo dummy de should_exit_thread_immediately
bool should_exit_thread_immediately() {
    static int counter = 0;
    counter++;
    return counter > 10; // Sale después de 10 iteraciones
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, nullptr, thread_function, nullptr);
    pthread_join(thread, nullptr);
    std::cout << "Thread finalizó correctamente.\n";
    return 0;
}

