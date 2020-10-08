#include "AbstractFactory.h"

#include <iostream>
#include <typeindex>
#include <functional>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include <type_traits>


class Shape {
public:
    explicit Shape(int arg1, const std::string& arg2) {}
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Line : public Shape {
public:
    explicit Line(int arg1, const std::string& arg2) : Shape(arg1, arg2) {}
    virtual void draw() const { std::cout << "Drawing line..." << std::endl; }
};

class Triangle : public Shape {
public:
    explicit Triangle(int arg1, const std::string& arg2) : Shape(arg1, arg2) {}
    virtual void draw() const { std::cout << "Drawing triangle..." << std::endl; }
};

class Circle : public Shape {
public:
    explicit Circle(int arg1, const std::string& arg2) : Shape(arg1, arg2) {}
    virtual void draw() const { std::cout << "Drawing circle..." << std::endl; }
};


 using ShapeFactory = Factory<Shape, std::type_index, std::function<Shape*(int, const std::string&)>, std::unordered_map>;

// Можем использовать следующим образом: using ShapeFactory = Factory<Shape, int, Shape*(*)(int, const std::string&), std::map>;
// вместо std::type_index, можем использовать int - целочисленный идентификатор
// вместо std::function<Shape*(int, const std::string&)> - можем использовать обычный указатель на функцию Shape*(*)(int, const std::string&)
// вместо std::unordered_map - можем использовать std::map или любой другой ассоциативный контейнер с похожим API


struct CreateTriangleFunctor {
    Triangle* operator()(int arg1, const std::string& arg2) const {
        return new Triangle(arg1, arg2);
    }
};

Shape* CreateCircleFunction(int arg1, const std::string& arg2) {
    return new Circle(arg1, arg2);
}


template<typename T, typename = typename std::enable_if<std::is_base_of<Shape, T>::value>::type>
struct ShapeCreator {
    Shape* operator()(int arg1, const std::string& arg2) const {
        return new T(arg1, arg2);
    }
};


int main()
{
    ShapeFactory factory;
    factory.Register(typeid(Line), ShapeCreator<Line>());
    factory.Register(typeid(Triangle), ShapeCreator<Triangle>());
    factory.Register(typeid(Circle), ShapeCreator<Circle>());

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.reserve(3);
    shapes.emplace_back(factory.Create(typeid(Line), 0, "Line"));
    shapes.emplace_back(factory.Create(typeid(Triangle), 1, "Triangle"));
    shapes.emplace_back(factory.Create(typeid(Circle), 2, "Circle"));

    for(const auto& shape : shapes)
        shape->draw();

    return 0;
}
