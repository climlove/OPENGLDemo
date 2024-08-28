#ifndef PANORAMARENDERER_H
#define PANORAMARENDERER_H

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include "Sphere.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif
// FFmpeg，音视频解码，同步，播放
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/channel_layout.h>
#include <libavutil/time.h>

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
// 全景视频，多线程处理
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include<iostream>
#include<string>
#include <utility>
#endif

// 安卓& log
#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <jni.h>

// Android AudioTrack
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <pthread.h>
#include <sched.h>

#define LOG_TAG "PanoramaRenderer"
#define printf(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else
#include <stdio.h>
#define LOGE(FORMAT, ...) printf(FORMAT, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
// glm高性能数学库
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
//#include <glm/gtx/norm.hpp>
#endif


#ifdef __cplusplus
extern "C" {
#endif
 void  processDecodedFrame(AVFrame* avFrame);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class MadgwickAHRS {
public:
    float samplePeriod;
    glm::quat quaternion;  // Quaternion describing the Earth relative to the sensor
    float beta;  // Algorithm gain

    MadgwickAHRS(float samplePeriod = 1.0f/256.0f, float beta = 1.0f)
            : samplePeriod(samplePeriod), beta(beta) {
        quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    void Update(const glm::vec3& gyroscope, const glm::vec3& accelerometer) {
        // Normalize accelerometer measurement
        glm::vec3 acc = glm::normalize(accelerometer);

        printf("gyroscope : (%.6f, %.6f, %.6f)", gyroscope.x, gyroscope.y, gyroscope.z);
        printf("Normalized accelerometer: (%.6f, %.6f, %.6f)", acc.x, acc.y, acc.z);

        // Gradient descent algorithm corrective step
        glm::vec3 F(
                2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y) - acc.x,
                2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z) - acc.y,
                2.0f * (0.5f - quaternion.x * quaternion.x - quaternion.y * quaternion.y) - acc.z
        );

        printf("F vector: (%.6f, %.6f, %.6f)", F.x, F.y, F.z);

        glm::mat4 J(
                -2.0f * quaternion.y,  2.0f * quaternion.z, -2.0f * quaternion.w,  2.0f * quaternion.x,
                2.0f * quaternion.x,   2.0f * quaternion.w,  2.0f * quaternion.z, -2.0f * quaternion.y,
                0.0f, -4.0f * quaternion.x, -4.0f * quaternion.y,  0.0f,
                0.0f, 0.0f, 0.0f, 0.0f
        );

        glm::vec4 F4 = glm::vec4(F, 0.0f);
        glm::vec4 step4 = glm::normalize(glm::transpose(J) * F4);

        printf("Step vector: (%.6f, %.6f, %.6f, %.6f)", step4.x, step4.y, step4.z, step4.w);

        // Compute rate of change of quaternion
        glm::quat qDot = 0.5f * quaternion * glm::quat(0.0f, gyroscope.x, gyroscope.y, gyroscope.z) - beta * glm::quat(step4);

        printf("qDot: (%.6f, %.6f, %.6f, %.6f)", qDot.w, qDot.x, qDot.y, qDot.z);

        // Integrate to yield quaternion
        quaternion += qDot * samplePeriod;
        quaternion = glm::normalize(quaternion);

        printf("Updated quaternion: (%.6f, %.6f, %.6f, %.6f)", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
    }

    glm::vec3 getEulerAngles() const {
        return glm::eulerAngles(quaternion);  // Returns the Euler angles (yaw, pitch, roll)
    }
};


class PanoramaRenderer {
public:
    static cv::Mat frame;  // 全景视频帧
    PanoramaRenderer();
    ~PanoramaRenderer();
    void processUI(cv::Mat matFrame);
    void onSurfaceCreated();
    void onDrawFrame();
    void onSurfaceChanged(int width, int height);

    void handleTouchDrag(float deltaX, float deltaY);
    void handlePinchZoom(float scaleFactor);
    void onGyroAccUpdate(float gyroX, float gyroY, float gyroZ,float accX,float accY,float accZ);

    //This method generates an external texture (using GL_TEXTURE_EXTERNAL_OES) and sets texture parameters. It is used to create the texture ID that is returned to the Kotlin side and used in SurfaceTexture.
    GLuint createExternalTexture();  // Create external texture for rendering video frames

    static void processDecodedFrameImpl(AVFrame* avFrame);
private:
    GLuint loadShader(GLenum type, const char *shaderSrc);
    GLuint createProgram(const char *vertexSrc, const char *fragmentSrc);

    void updateVideoFrame();  // Update texture with video frame from SurfaceTexture


    // 全景图像需要的函数
    GLuint loadTexture(const char *assetPath);

    // 全景图片和视频渲染
    GLuint shaderProgram;
    GLuint texture; // 全景图片纹理
    GLuint vao, vboVertices, vboTexCoords,vboIndices;
    SphereData* sphereData;
     
    int frameWidth,frameHeight; //全景视频帧的宽和高
    GLuint videoTexture; // 全景视频帧纹理
    cv::VideoCapture videoCapture; // 使用OpenCV解码

    static std::mutex textureMutex; // 纹理线程锁
   // AAssetManager* assetManager;
    std::string sharePath; // 共享文件夹，具有读写权限, JNI传入
    glm::mat4 projection;
    glm::mat4 view;
    float rotationX,rotationY,zoom;

    // 手机陀螺仪
    MadgwickAHRS ahrs;

    // 播放屏幕宽和高尺寸
    int widthScreen;
    int heightScreen;

};
#endif
#endif
