#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <iostream>

class SimpleBox
{
  public:
    SimpleBox()
    {};

    SimpleBox(dWorldID world, dSpaceID space, dVector3 sides)
    {

        for (int i = 0; i < 3; i++)
        {
            this->sides[i] = sides[i];
        }

        body = dBodyCreate(world);
        geom = dCreateBox(space, sides[0], sides[1], sides[2]);
        dMass mass;
        dMassSetZero(&mass);
        dMassSetBox(&mass, 1000, sides[0], sides[1], sides[2]);
        dBodySetMass(body, &mass);

        dGeomSetBody(geom, body);
    };

    void draw()
    {
        const dReal *pos = dBodyGetPosition(body);
        const dReal *rot = dBodyGetRotation(body);
        dsDrawBoxD(pos, rot, sides);
    };

    void setPosition(dReal x, dReal y, dReal z)
    {
        dBodySetPosition(body, x, y, z);
    }

  private:
    dBodyID body;
    dGeomID geom;
    dVector3 sides;
};
