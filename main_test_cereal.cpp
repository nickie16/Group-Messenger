#include <string>
#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

using std::string;
using std::cout;

class SomeData
{
public:
    SomeData() = default;

    SomeData(int a, int b, string c) : a(a), b(b), c(c) {}

    int getA() const {
        return a;
    }

    void setA(int a) {
        SomeData::a = a;
    }

    int getB() const {
        return b;
    }

    void setB(int b) {
        SomeData::b = b;
    }

    const string &getC() const {
        return c;
    }

    void setC(const string &c) {
        SomeData::c = c;
    }

private:
    int a{};
    int b{};
    string c;
    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(a, b, c);
    }

    // another way to serialize this object both work properly
//    template <class Archive>
//    void save( Archive & ar) const
//    {
//        ar( a, b ); // operator() is the preferred way of interfacing the archive
//    }
//
//    template <class Archive>
//    void load( Archive & ar)
//    {
//        ar( a, b );
//    }

};


int main(){

    string input;

    {
        std::stringstream ss;
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

        SomeData m1(1,2, "c");
        oarchive(m1); // Write the data to the archive
        input = ss.str();
    } // archive goes out of scope, ensuring all contents are flushed

    {
        std::stringstream ss; // any stream can be used
        ss << input;
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive

        SomeData m1;
        iarchive(m1); // Read the data from the archive

        cout << m1.getA() << " " << m1.getB() << " " << m1.getC();

    }
    return 0;
}
