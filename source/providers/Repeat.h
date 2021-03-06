#ifndef PROVIDERS_REPEAT_H
#define PROVIDERS_REPEAT_H

#include "StreamProvider.h"

#include "../Utility.h"

namespace stream {
namespace provider {

template<typename T>
class Repeat : public StreamProvider<T> {

public:
    Repeat(const T& value) : value_(std::make_shared<T>(value)) {}

    Repeat(T&& value) : value_(std::make_shared<T>(value)) {}

    std::shared_ptr<T> get() override {
        return value_;
    }

    bool advance_impl() override {
        return true;
    }

    std::pair<int, int> print(std::ostream& os, int indent) const override {
        this->print_indent_arrow(os, indent);
        os << "[repeated value stream]\n";
        return {0, 1};
    }

private:
    std::shared_ptr<T> value_;

};

} /* namespace provider */
} /* namespace stream */

#endif
