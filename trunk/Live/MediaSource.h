﻿#ifndef BXC_RTSPSERVER_MEDIASOURCE_H
#define BXC_RTSPSERVER_MEDIASOURCE_H
#include <queue>
#include <mutex>
#include <stdint.h>
#include "../Scheduler/UsageEnvironment.h"
#include "../Scheduler/ThreadPool.h"


#define FRAME_MAX_SIZE (1024*200)
#define DEFAULT_FRAME_NUM 4

class MediaFrame
{

public:
    MediaFrame() :
        temp(new uint8_t[FRAME_MAX_SIZE]),// 创建一个大小为FRAME_MAX_SIZE的临时数组
        mBuf(nullptr),// 初始化mBuf为空指针
        mSize(0){// 初始化mSize为0
        
    }
    ~MediaFrame(){
        delete []temp;
    }
    
    uint8_t* temp;// 容器
    uint8_t* mBuf;// 引用容器
    int mSize;
};

class MediaSource
{

public:

    explicit MediaSource(UsageEnvironment* env); //构造函数，接受一个UsageEnvironment指针作为参数
    virtual ~MediaSource();

    MediaFrame* getFrameFromOutputQueue();//从输出队列获取帧
    void putFrameToInputQueue(MediaFrame* frame); // 把帧送入输入队列
    int getFps() const { return mFps; }
    std::string getSourceName(){ return mSourceName;}

private:
    static void taskCallback(void* arg);
protected:
    virtual void handleTask() = 0;
    void setFps(int fps) { mFps = fps; }

protected:
    UsageEnvironment* mEnv;
    MediaFrame mFrames[DEFAULT_FRAME_NUM];
    std::queue<MediaFrame*> mFrameInputQueue;
    std::queue<MediaFrame*> mFrameOutputQueue;


    std::mutex mMtx;
    ThreadPool::Task mTask;
    int mFps;
    std::string mSourceName;

};
#endif //BXC_RTSPSERVER_MEDIASOURCE_H