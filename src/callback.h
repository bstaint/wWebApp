#ifndef CALLBACK_H
#define CALLBACK_H

#include "wke.h"

// 执行一些窗口相关操作
// mousedown: 移动窗口
// quit: 退出程序
jsValue exeCallback(jsExecState es, void *param);

// 使用默认浏览器打开url
jsValue urlCallback(jsExecState es, void *param);

#endif // CALLBACK_H
