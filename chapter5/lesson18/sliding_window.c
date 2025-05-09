/*
    TCP 滑动窗口：是网络拥塞控制的一种手段，因为受限于数据接收方处理数据不够快的缘故，所以需要滑动窗口来限制发送方发送数据的速度
    - 滑动窗口：顾名思义，就是在进行通信的过程中，数据接收方的缓冲区是在不断变化的，决定了接收方当前最多能接收多少数据
    - 在进行 TCP 通信时，接收方在给发送方发送 ACK=1 确认报文段的时候，会指定当前缓冲区的大小，发送方根据缓冲区大小来发送数据

    mss：最大报文段大小 (Maximum Segment Size)
*/