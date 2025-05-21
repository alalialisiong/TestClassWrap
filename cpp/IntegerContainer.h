#pragma once
#include <vector>
#include <functional>

class IntegerContainer {
private:
    int* m_data;
    size_t m_size;

public:
    IntegerContainer(size_t size = 10);
    ~IntegerContainer();
    
    // 데이터 접근 메서드
    int getValue(size_t index) const;
    void setValue(size_t index, int value);
    
    // 데이터 포인터 직접 접근 (메모리 공유용)
    int* data();
    size_t size() const;
    
    // 비동기 처리 예시 - 모든 값에 연산 수행
    void processAsync(int addValue, std::function<void(bool)> callback);
};
