//
//  sport.cpp
//  Problem1
//
//  Created by Gautam Gupta on 2/5/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

class Sport
{
public:
    Sport(string param) : m_name(param) {}
    virtual ~Sport() {}
    string name() const { return m_name; }
    virtual string icon() const = 0;
    virtual bool isOutdoor() const { return true; }
private:
    string m_name;
    bool m_isOutdoor;
};

class Snowboarding : public Sport
{
public:
    Snowboarding(string name) : Sport(name)
    {}
    ~Snowboarding()
    {
        cout << "Destroying the Snowboarding object named "<< name() << "." << endl;
    }
    string icon() const
    {
        return "a descending snowboarder";
    }
};
class Biathlon : public Sport
{
public:
    Biathlon(string name, double distance)
    : Sport(name), m_distance(distance)
    {}
    ~Biathlon()
    {
        cout << "Destroying the Biathlon object named " << name() << ", distance " << distance() << " km." << endl;
        
    }
    string icon() const
    {
        return "a skier with a rifle";
    }
    double distance() { return m_distance; }
    
private:
    double m_distance;
};

class FigureSkating : public Sport
{
public:
    FigureSkating(string name)
    : Sport(name)
    {}
    ~FigureSkating()
    {
        cout << "Destroying the FigureSkating object named "<< name() << "." << endl;
    }
    bool isOutdoor() const { return false; }
    string icon() const
    {
        return "a skater in the Biellmann position";
    }
};
