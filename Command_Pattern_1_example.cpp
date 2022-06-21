#include <iostream>
#include <vector>


//Abstrac class to associate button next. 
class Command
{
 public:
 Command(){};
  //not need constructor 
    virtual ~Command() = default;
    virtual void functionToExecute() const = 0;
    int ID;
    bool pressed =1;

};

class ReloadWeapon : public Command 
{

 private:
 void reload() const {std::cout<<"Reloading.......\n";}

 public:
  virtual ~ReloadWeapon() = default;
 virtual void functionToExecute() const override final { reload();}
 int ID = 1;
};

class HabilityOne : public Command 
{

 private:
 void habilityOne() const {std::cout<<"Hability player \n";}

 public:
 virtual ~HabilityOne() = default;
 virtual  void functionToExecute() const override final { habilityOne();}
 int ID = 2; 
};

class Jump: public Command 
{

 private:
 void jump() const {std::cout<<"Jumping.......\n";}

 public:
 virtual ~Jump() = default;
 virtual void  functionToExecute() const override final {jump();}
 int ID = 3;
  
};

class ChangeWeapon : public Command 
{
 private:
 void changeWeapon() const {std::cout<<"changing weapon.......\n";}

 public:
 virtual ~ChangeWeapon() = default;
 virtual void functionToExecute() const override final { changeWeapon();}
 int ID = 4; 
};


class Controller{

    public:

    Controller() {};

    void InputController(){

        if(isPressed(ButtonX))ButtonX->functionToExecute();
        if(isPressed(ButtonY))ButtonY->functionToExecute();
        if(isPressed(ButtonB))ButtonB->functionToExecute();
        if(isPressed(ButtonA))ButtonA->functionToExecute();

    }

   
private:
    Command* ButtonX = new  ReloadWeapon;
    Command* ButtonY = new HabilityOne;
    Command* ButtonA = new ChangeWeapon;
    Command* ButtonB = new Jump;

    bool 
    isPressed(Command* button){
        return button->pressed;
    }

};


int main(){

Controller control;

control.InputController();


}
