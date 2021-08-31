#ifndef IARGUMETNS_H
#define IARGUMETNS_H

struct arguments;

class IArguments
{
public:
    IArguments() = default;
    virtual ~IArguments() = default;

    virtual void init      (const arguments & arg_set)         = 0;
    virtual int  setArgs   (const char ** args, const int & n) = 0;
    
    virtual bool getInt    (const std::string & arg_name, int & value)         = 0;
    virtual bool getDouble (const std::string & arg_name, double & value)      = 0;
    virtual bool getString (const std::string & arg_name, std::string & value) = 0;
};

#endif // IARGUMETNS_H