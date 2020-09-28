#pragma once

#define _SAFE_DELETE_(_ptr_) do { delete _ptr_; _ptr_ = nullptr; } while(false)

