#include "IntegerContainer.h"
#include <thread>
#include <chrono>

IntegerContainer::IntegerContainer(size_t size) : m_size(size) {
    m_data = new int[size]();  // 0으로 초기화
}

IntegerContainer::~IntegerContainer() {
    delete[] m_data;
}

int IntegerContainer::getValue(size_t index) const {
    if (index < m_size) {
        return m_data[index];
    }
    return 0;
}

void IntegerContainer::setValue(size_t index, int value) {
    if (index < m_size) {
        m_data[index] = value;
    }
}

int* IntegerContainer::data() {
    return m_data;
}

size_t IntegerContainer::size() const {
    return m_size;
}

void IntegerContainer::processAsync(int addValue, std::function<void(bool)> callback) {
    // 비동기 작업 시뮬레이션
    std::thread([this, addValue, callback]() {
        // 시간이 걸리는 작업 시뮬레이션
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        // 모든 값에 addValue 더하기
        for (size_t i = 0; i < m_size; i++) {
            m_data[i] += addValue;
        }
        
        // 작업 완료 콜백
        callback(true);
    }).detach();
}
