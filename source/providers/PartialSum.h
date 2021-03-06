#ifndef PROVIDERS_PARTIAL_SUM_H
#define PROVIDERS_PARTIAL_SUM_H

#include "StreamProvider.h"

namespace stream {
namespace provider {

template<typename T, typename Adder>
class PartialSum : public StreamProvider<T> {

public:
    PartialSum(StreamProviderPtr<T> source, Adder&& add)
        : source_(std::move(source)), add_(add) {}

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance_impl() override {
        if(first_) {
            first_ = false;
            if(source_->advance()) {
                current_ = source_->get();
                return true;
            }
            return false;
        }

        if(source_->advance()) {
            current_ = std::make_shared<T>(add_(
                std::move(*current_),
                std::move(*source_->get())));
            return true;
        }
        current_.reset();
        return false;
    }

    std::pair<int, int> print(std::ostream& os, int indent) const override {
        this->print_indent_arrow(os, indent);
        os << "PartialSum:\n";
        auto sub = source_->print(os, indent + 1);
        return {sub.first + 1, sub.second};
    }

private:
    StreamProviderPtr<T> source_;
    Adder add_;
    std::shared_ptr<T> current_;
    bool first_ = true;
};

} /* namespace provider */
} /* namespace stream */

#endif