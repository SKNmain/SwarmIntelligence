#pragma once

#define DELLPTR(ptr)                    \
if(nullptr != ptr)                      \
{                                       \
   delete ptr;                          \
   ptr = nullptr;                       \
}                                       \

#define DELL_QT_PTR(ptr)                \
if(nullptr != ptr)                      \
{                                       \
   ptr->deleteLater();                  \
   ptr = nullptr;                       \
}                                       