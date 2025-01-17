#include "objects/TruckWheelObject.h"

WheelObject::WheelObject(glm::vec3 trans, float scal)
    : Object(trans, scal) {

}

WheelObject::~WheelObject() {

}

void WheelObject::draw(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    M = glm::translate(M,translate);
    M = glm::rotate(M,angle_dr,glm::vec3(0.0f,1.0f,0.0f));
    if(angle_rot!=0)
        M = glm::rotate(M,-angle_rot,glm::vec3(0.0f,0.0f,1.0f));
    M = glm::scale(M,glm::vec3(scale,scale,scale));

    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
    model->drawSolid();

}

bool WheelObject::initialize_model() { // Model trzeba zainicjowac w initOpenGLProgram
    model = new Models::Model("models/wheel.obj", 0); // Sciezka modelu, id_tekstury
    return 0;
}

void WheelObject::destroy_model() { // Model trzeba usunac w freeOpenGLProgram
    delete model;
}
