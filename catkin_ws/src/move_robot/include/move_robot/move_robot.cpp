//Bibliotecas necesarias de ROS
#include <ros/ros.h> //Bliblioteca necesaria para utilizar ROS  
#include <geometry_msgs/Twist.h> //Definición de un mensaje de ROS llamado Twist que se utiliza para representar la velocidad lineal y angular de un objeto.

//Bibliotecas de C++ necesarias para interactuar con el teclado y habilitar la entrada no bloqueante.
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

//Esta línea define una función llamada keyboardCallback() se llama cada vez que se recibe un mensaje de tipo Twist en el topic /cmd_vel. El parámetro msg contiene el mensaje recibido.
void keyboardCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    //Inicialización del teclado y habilitación de la entrada no bloqueante del mismo para que el programa pueda recibir eventos de teclado de manera inmediata.
    int kfd = 0;
    struct termios cooked, raw;
    ros::Publisher pub;
    geometry_msgs::Twist vel;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "caja_teleop");
    ros::NodeHandle n; //Esta línea crea un objeto de tipo ros::NodeHandle que se utiliza para manejar la comunicación con ROS.
    pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1); //Esta línea crea un objeto de tipo ros::Publisher que se utiliza para publicar mensajes de tipo Twist en el topic /cmd_vel. El número 1000 indica el tamaño máximo de la cola de mensajes.

    tcgetattr(kfd, &cooked);
    memcpy(&raw, &cooked, sizeof(struct termios));
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VEOL] = 1;
    raw.c_cc[VEOF] = 2;
    tcsetattr(kfd, TCSANOW, &raw);
    fcntl(kfd, F_SETFL, O_NONBLOCK);

    ROS_INFO("Mueve la caja con las teclas 'w', 'a', 's', 'd'.");
    ROS_INFO("Presiona Ctrl-C para salir.");

//Este bucle infinito lee la entrada del teclado y procesa la tecla presionada utilizando un switch. El programa se mantiene en este bucle hasta que se cierre el nodo de ROS.
    while(ros::ok())
    {
        char c;
        if(read(kfd, &c, 1) < 0)
        {
            continue;
        }

        vel.linear.x = vel.linear.y = vel.linear.z = 0;
        vel.angular.x = vel.angular.y = vel.angular.z = 0;

        switch(c)
        {
            case 'w':
                vel.linear.x = 0.5;
                break;
            case 'a':
                vel.angular.z = 1.0;
                break;
            case 's':
                vel.linear.x = -0.5;
                break;
            case 'd':
                vel.angular.z = -1.0;
                break;
        }

        pub.publish(vel);
        ros::spinOnce();
    }

    tcsetattr(kfd, TCSANOW, &cooked);

    return 0;
}
