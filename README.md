simple_thread_pool
==================
一个简单的线程池实现。
### 注意
线程池成员函数async的每一个参数均会有至少两次copy构造的开销，可使用reference wrapper (std::ref/std::cref)传参避免此开销。
