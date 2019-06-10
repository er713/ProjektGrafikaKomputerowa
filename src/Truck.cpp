#include "Truck.h"

Truck::Truck() : Object(glm::vec3(0.0f,0.0f,11.0f))
{
    wheel_l = new Object(glm::vec3(10.0f,-10.0f,-10.0f));
    wheel_r = new Object(glm::vec3(10.0f,10.0f,-10.0f),1.0f,PI);
    main_part = new Object(glm::vec3(0.0f,0.0f,0.0f), 10.0f);
    back_part = new Object(glm::vec3(-21.0f,0.0f,0.0f), 10.0f);
    wheels[0] = new Object(glm::vec3(-10.0f,-10.0f,-10.0f));
    wheels[1] = new Object(glm::vec3(-20.0f,-10.0f,-10.0f));
    wheels[2] = new Object(glm::vec3(-22.0f,-10.0f,-10.0f));
    wheels[3] = new Object(glm::vec3(-10.0f,10.0f,-10.0f),1.0f,PI);
    wheels[4] = new Object(glm::vec3(-20.0f,10.0f,-10.0f),1.0f,PI);
    wheels[5] = new Object(glm::vec3(-22.0f,10.0f,-10.0f),1.0f,PI);

    speed = 0.0f;
    acceleration = 0.0f;
    b_acc = false;
    turn_l = turn_r = 0.0f;
}

Truck::~Truck()
{
    wheel_l->~Object();
    wheel_r->~Object();
    main_part->~Object();
    back_part->~Object();
    wheels[0]->~Object();
    wheels[1]->~Object();
    wheels[2]->~Object();
    wheels[3]->~Object();
    delete[] wheels;
    delete wheel_l;
    delete wheel_r;
    delete main_part;
    delete back_part;
}

void Truck::draw_all(glm::mat4 P, glm::mat4 V){
    M = glm::translate(glm::mat4(1.0f),translate
                       /*+glm::vec3(wheel_odlegl/2*(cos(angle_dr)-1),wheel_odlegl/2*sin(angle_dr),0.0f)*/);
    M = glm::rotate(M,angle_dr,glm::vec3(0.0f,0.0f,1.0f));
    printf("%f, %f, %f, %f\n", translate.x,translate.y, translate.z,angle_dr);

    wheel_l->draw(P,V,M);
    wheel_r->draw(P,V,M);
    main_part->draw(P,V,M);
    this->wheels_draw(P,V,M);
    //back_part->draw(P,V,M);
}

void Truck::update(double time){

    wheel_l->angle_dr+=(turn_l-turn_r)*time;
    if(wheel_l->angle_dr>angle_max) wheel_l->angle_dr=angle_max;
    else if(wheel_l->angle_dr<-angle_max) wheel_l->angle_dr = -angle_max;
    wheel_r->angle_dr=wheel_l->angle_dr;

    if(wheel_l->angle_dr!=0){
        ctga = 1/tan(wheel_l->angle_dr);
        R = 2*wheel_odlegl * ctga;
        s = speed*time + (acceleration-this->friction())*time*time/2;
        omega = s/R;
        px = R*sin(omega);
        py = R*(1-cos(omega));
        delt = omega; //albo omega, albo omega/2


        translate += glm::rotateZ(glm::vec3(px,-py,0.0f),this->angle_dr);

        this->angle_dr += delt;
        if(this->angle_dr>2*PI)this->angle_dr-=2*PI;
        else if(this->angle_dr<-2*PI)this->angle_dr+=2*PI;

        //bycmoze potrzebna rozna predkosc kol po obu stronach, zeby to wygladalo estetyczniej, albo po prostu potrzebuje dopracowania
        wheel_l->angle_rot += speed*(R+wheel_rozstaw/2)/R*float(time); //na razie r=1, wiec predkosc katowa=liniowej
        if(wheel_l->angle_rot>2*PI)
            wheel_l->angle_rot-=2*PI;
        wheel_r->angle_rot += speed*(R-wheel_rozstaw/2)/R*float(time);
        if(wheel_r->angle_rot>2*PI)
            wheel_r->angle_rot-=2*PI;
        this->wheels_round(wheel_l->angle_rot, wheel_r->angle_rot);

    }
    else{
        s = speed*time + (acceleration-this->friction())*time*time/2;

        translate += glm::rotateZ(glm::vec3(s,0.0f,0.0f),this->angle_dr);

        wheel_l->angle_rot += speed*float(time); //na razie r=1, wiec predkosc katowa=liniowej
        if(wheel_l->angle_rot>2*PI)
            wheel_l->angle_rot-=2*PI;
        wheel_r->angle_rot = wheel_l->angle_rot;
        this->wheels_round(wheel_l->angle_rot);

    }
    speed += (acceleration-this->friction())*float(time);

    //printf("%f, %f\n", speed,time);

}

void Truck::wheels_round(float angle, float op_angle){

    wheels[0]->angle_rot=angle;
    wheels[3]->angle_rot=PI-op_angle;


    //wheels[1]->angle_rot=angle;
    //wheels[2]->angle_rot=angle;
    //wheels[5]->angle_rot=PI-op_angle;
    //wheels[4]->angle_rot=PI-op_angle;

}

void Truck::wheels_round(float angle){

    wheels[0]->angle_rot=angle;
    wheels[3]->angle_rot=PI-angle;


    //wheels[1]->angle_rot=angle;
    //wheels[2]->angle_rot=angle;
    //wheels[4]->angle_rot=PI-angle;
    //wheels[5]->angle_rot=PI-angle;

}

void Truck::wheels_draw(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    wheels[0]->draw(P,V,M);
    wheels[3]->draw(P,V,M);


    //wheels[1]->draw(P,V,M);
    //wheels[2]->draw(P,V,M);
    //wheels[4]->draw(P,V,M);
    //wheels[5]->draw(P,V,M);

}

//TODO jakies ladne tarcie ogrnac
float Truck::friction(){
    return speed/7.0f;
}
