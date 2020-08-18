
class Vector {
public:
    Vector(int s);
    double& operator[](int i);
    int size();

private:
    double* elem; // pointer to the elements
    int sz; // the number of elements
};