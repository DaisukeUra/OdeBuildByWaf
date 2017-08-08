#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include "SimpleBox.h"

/* ds Macros */

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

/* ODE objects */

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static SimpleBox box;

/* this is called by dSpaceCollide when two objects in space are
 * potentially colliding.
 */

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
  /* exit without doing anything if the two bodies are connected by a joint */
  dBodyID b1, b2;
  dContact contact;

  b1 = dGeomGetBody(o1);
  b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnected(b1, b2))
    return;

  contact.surface.mode = 0;
  contact.surface.mu = 0.1;
  contact.surface.mu2 = 0;
  if (dCollide(o1, o2, 1, &contact.geom, sizeof(dContactGeom)))
  {
    dJointID c = dJointCreateContact(world, contactgroup, &contact);
    dJointAttach(c, b1, b2);
  }
}

/* start simulation - set viewpoint */

static void start()
{
  static float xyz[3] = {2.1640f, -1.3079f, 1.7600f};
  static float hpr[3] = {125.5000f, -17.0000f, 0.0000f};

  dAllocateODEDataForThread(dAllocateMaskAll);
  dsSetViewpoint(xyz, hpr);
}

/* simulation loop */

static void simLoop(int pause)
{
  int i;
  if (!pause)
  {

    dSpaceCollide(space, 0, &nearCallback);
    dWorldStep(world, 0.05);

    /* remove all contact joints */
    dJointGroupEmpty(contactgroup);
  }

  dsSetColor(1, 1, 0);
  box.draw();
}

int main(int argc, char **argv)
{
  int i;
  dReal k;
  dMass m;

  /* setup pointers to drawstuff callback functions */
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = 0;
  fn.stop = 0;
  fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;

  /* create world */
  dInitODE2(0);
  world = dWorldCreate();
  space = dHashSpaceCreate(0);
  contactgroup = dJointGroupCreate(0);
  dWorldSetGravity(world, 0, 0, -0.5);
  dCreatePlane(space, 0, 0, 1, 0);

  /* create box */
  dVector3 sides = {1, 1, 1, 1};
  box = SimpleBox(world, space, sides);
  box.setPosition(0,0,10);

  /* run simulation */
  dsSimulationLoop(argc, argv, 300, 200, &fn);

  dJointGroupDestroy(contactgroup);
  dSpaceDestroy(space);
  dWorldDestroy(world);
  dCloseODE();
  return 0;
}
