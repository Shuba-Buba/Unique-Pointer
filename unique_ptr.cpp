#pragma once

#include <cstddef>  // std::nullptr_t
#include <utility>
#include <iostream>
#include "compressed_pair.h"

#include <catch.hpp>
#include <type_traits>
#include <memory>
#include <mutex>

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
    CompressedPair<T*, Deleter> own_;

public:
    explicit UniquePtr(T* ptr = nullptr) {
        own_.GetFirst() = ptr;
    }

    template <class Deleter1>
    UniquePtr(T* ptr, Deleter1&& deleter) noexcept : own_(ptr, std::forward<Deleter1>(deleter)) {
    }

    UniquePtr(UniquePtr& other) = delete;

    UniquePtr(UniquePtr&& other) noexcept : own_(other.Release(), std::move(other.GetDeleter())) {
    }

    template <class U, class E>
    UniquePtr(UniquePtr<U, E>&& other) noexcept
        : own_(other.Release(), std::move(other.GetDeleter())) {
    }

    UniquePtr& operator=(UniquePtr& other) = delete;

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (own_.GetFirst() == other.own_.GetFirst()) {
            return *this;
        }

        if (own_.GetFirst()) {
            own_.GetSecond()(own_.GetFirst());
        }
        own_.GetFirst() = other.own_.GetFirst();
        own_.GetSecond() = std::forward<Deleter>(other.own_.GetSecond());
        other.own_.GetFirst() = nullptr;
        return *this;
    }

    UniquePtr& operator=(std::nullptr_t) {
        if (own_.GetFirst()) {
            own_.GetSecond()(own_.GetFirst());
        }
        own_.GetFirst() = nullptr;
        return *this;
    }

    ~UniquePtr() {
        if (own_.GetFirst()) {
            own_.GetSecond()(own_.GetFirst());
        }
    }

    T* Release() {
        auto alive = own_.GetFirst();
        own_.GetFirst() = nullptr;
        return alive;
    }

    void Reset(T* ptr = nullptr) {
        auto to = own_.GetFirst();
        own_.GetFirst() = ptr;
        if (to) {
            own_.GetSecond()(to);
        }
    }

    void Swap(UniquePtr& other) {
        std::swap(own_, other.own_);
    }

    T* Get() const {
        return own_.GetFirst();
    }

    Deleter& GetDeleter() {
        return own_.GetSecond();
    }

    const Deleter& GetDeleter() const {
        return own_.GetSecond();
    }

    explicit operator bool() const {
        return (own_.GetFirst());
    }

    std::add_lvalue_reference_t<T> operator*() const {
        return *own_.GetFirst();
    }

    T* operator->() const {
        return own_.GetFirst();
    }
};

template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
private:
    CompressedPair<T*, Deleter> own_;

public:
    explicit UniquePtr(T* ptr = nullptr) {
        own_.GetFirst() = ptr;
    }

    template <class Deleter1>
    UniquePtr(T* ptr, Deleter1&& deleter) noexcept : own_(ptr, std::forward<Deleter1>(deleter)) {
    }

    UniquePtr(UniquePtr& other) = delete;

    UniquePtr(UniquePtr&& other) noexcept : own_(other.Release(), std::move(other.GetDeleter())) {
    }

    template <class U, class E>
    UniquePtr(UniquePtr<U, E>&& other) noexcept
        : own_(other.Release(), std::move(other.GetDeleter())) {
    }

    UniquePtr& operator=(UniquePtr& other) = delete;

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (own_.GetFirst() == other.own_.GetFirst()) {
            return *this;
        }

        if (own_.GetFirst()) {
            own_.GetSecond()(own_.GetFirst());
        }
        own_.GetFirst() = other.own_.GetFirst();
        own_.GetSecond() = std::forward<Deleter>(other.own_.GetSecond());
        other.own_.GetFirst() = nullptr;
        return *this;
    }

    UniquePtr& operator=(std::nullptr_t) {
        if (own_.GetFirst()) {
            own_.GetSecond()(own_.GetFirst());
        }
        own_.GetFirst() = nullptr;
        return *this;
    }

    ~UniquePtr() {
        if (own_.GetFirst()) {
            own_.GetSecond()(own_.GetFirst());
        }
    }

    T* Release() {
        auto alive = own_.GetFirst();
        own_.GetFirst() = nullptr;
        return alive;
    }

    void Reset(T* ptr = nullptr) {
        auto to = own_.GetFirst();
        own_.GetFirst() = ptr;
        if (to) {
            own_.GetSecond()(to);
        }
    }

    void Swap(UniquePtr& other) {
        std::swap(own_, other.own_);
    }

    T* Get() const {
        return own_.GetFirst();
    }

    Deleter& GetDeleter() {
        return own_.GetSecond();
    }

    const Deleter& GetDeleter() const {
        return own_.GetSecond();
    }

    explicit operator bool() const {
        return (own_.GetFirst());
    }
    std::add_lvalue_reference_t<T> operator*() const {
        return *own_.GetFirst();
    }

    T* operator->() const {
        return own_.GetFirst();
    }

    const T& operator[](int i) const {
        return own_.GetFirst()[i];
    }

    T& operator[](int i) {
        return own_.GetFirst()[i];
    }
};

