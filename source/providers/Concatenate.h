#ifndef PROVIDERS_CONCATENATE_H
#define PROVIDERS_CONCATENATE_H

#include "StreamProvider.h"

#include <list>

namespace stream {
namespace provider {

template<typename T>
class Concatenate : public StreamProvider<T> {

public:
    template<typename Iterator>
    Concatenate(Iterator begin, Iterator end) : sources_(begin, end) {}

    Concatenate(StreamProviderPtr<T> first, StreamProviderPtr<T> second) {
        sources_.push_back(std::move(first));
        sources_.push_back(std::move(second));
    }

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance_impl() override {
        while(!sources_.empty()) {
            auto& provider = sources_.front();
            if(provider->advance()) {
                current_ = provider->get();
                return true;
            }
            sources_.pop_front();
        }
        current_.reset();
        return false;
    }

    void concat(StreamProviderPtr<T>&& source) {
        sources_.push_back(std::move(source));
    }

    std::pair<int, int> print(std::ostream& os, int indent) const override {
        this->print_indent_arrow(os, indent);
        os << "Concatenation[" << sources_.size() << "]:\n";
        int stages = 1;
        int sources = 0;
        for(auto& source : sources_) {
            auto sub = source->print(os, indent + 1);
            stages += sub.first;
            sources += sub.second;
        }
        return {stages, sources};
    }

private:
    std::list<StreamProviderPtr<T>> sources_;
    std::shared_ptr<T> current_;

};

} /* namespace provider */
} /* namespace stream */

#endif
