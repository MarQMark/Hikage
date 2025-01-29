#ifndef HIKAGE_VIEWABLE_H
#define HIKAGE_VIEWABLE_H

class Viewable {
public:
    virtual void render() = 0;
    void* view{};

    virtual void setVisible(bool visible) {
        _visible = visible;
    };
    bool isVisible() const{
        return _visible;
    }

protected:
    bool _visible = true;
};

#endif //HIKAGE_VIEWABLE_H
