/* **************************************************************************
ASWTools_Common.h
Author: Anthony S. West - ASW Software

Contains functions for creating a console window from a WinMain app.

Every attempt should be made to keep this module at least Windows portable.

Copyright 2025 Anthony S. West

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************** */

//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#ifndef ASWTools_CommonH
#define ASWTools_CommonH
//---------------------------------------------------------------------------
#include <cstddef>
#include <stdlib.h>
#include <windows.h>
//---------------------------------------------------------------------------

#if __cplusplus < 201103L
  #define nullptr NULL
  #define override
#endif

//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// RAII_Handle
/////////////////////////////////////////////////////////////////////////////
class RAII_Handle
{
private:
    HANDLE m_handle;

public:
    HANDLE GetHandle()
    {
        return m_handle;
    }

public:
    RAII_Handle(HANDLE handle)
        : m_handle(handle)
    {
    }

    ~RAII_Handle()
    {
        if (INVALID_HANDLE_VALUE != m_handle && nullptr != m_handle)
            ::CloseHandle(m_handle);
    }

    HANDLE Release()
    {
        HANDLE handle = m_handle;
        m_handle = INVALID_HANDLE_VALUE;
        return handle;
    }
};


/////////////////////////////////////////////////////////////////////////////
// RAII_LPVOID_Malloc
/////////////////////////////////////////////////////////////////////////////
class RAII_LPVOID_Malloc
{
private:
    size_t m_size;
    LPVOID m_lpData;

public:
    LPVOID GetData()
    {
        return m_lpData;
    }

    size_t GetSize()
    {
        return m_size;
    }

public:
    RAII_LPVOID_Malloc(size_t size)
        : m_lpData(nullptr)
    {
        m_size = size;
        m_lpData = static_cast<LPVOID>(malloc(m_size));
    }

    ~RAII_LPVOID_Malloc()
    {
        free(m_lpData);
    }

    LPVOID Release()
    {
        LPVOID data = m_lpData;
        m_lpData = nullptr;
        return data;
    }
};


/////////////////////////////////////////////////////////////////////////////
// RAII_MutexLock
/////////////////////////////////////////////////////////////////////////////
class RAII_MutexLock
{
private:
    bool m_leave;
    CRITICAL_SECTION& m_mutex;

public: // Getters/Setters
    CRITICAL_SECTION& GetMutex()
    {
        return m_mutex;
    }

public:
    RAII_MutexLock(CRITICAL_SECTION& mutex)
        : m_leave(true),
          m_mutex(mutex)
    {
        ::EnterCriticalSection(&m_mutex);
    }

    ~RAII_MutexLock()
    {
        if (m_leave)
            ::LeaveCriticalSection(&m_mutex);
    }

    CRITICAL_SECTION& Release()
    {
        m_leave = false;
        return m_mutex;
    }
};

} // namespace ASWTools

#endif // #ifndef ASWTools_CommonH
