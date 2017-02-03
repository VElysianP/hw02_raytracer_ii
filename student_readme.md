Ray Tracer Part I
======================

**University of Pennsylvania, CIS 561: Advanced Computer Graphics, Homework 1**

* Yaoyi (Elyson) Bai
* byaoyi

### TODO: Your README
Include renders of the provided scene files, and list any extra credit you
implemented.

# **Implementation** #
**1. Multithreading**

*To make sure that rendertask.cpp can work, please make sure to change the path of the first line of the rendertask.cpp and also the mygl.cpp file to match the place where you store the rendertask.h file.*


And you will see the result far more faster than the normal rendering process.


**2. Phong Material and Reflection**

In this part, the reflection and the Phong material are implemented through recursion between the EvaluateReflectedEnergy function in Lambert and Phong material class, and also the TraceRay function in Integrator. And we will have. We can see that there is no apparent differences if there is no Phong material in the scene between the new scene and HW1.

![](http://i.imgur.com/Zgpw9dc.png)
![](http://i.imgur.com/12RNghE.png)
![](http://i.imgur.com/IPDVlXR.png)
![](http://i.imgur.com/MHpBDhx.png)

**3. Specular Transmission**

In this part, the transmission of transparent objects are implemented to display the reflection.

![](http://i.imgur.com/FGNsiqK.png)

As you can see there are several bugs inside, in my opinion there might be some issues about 1) the camera displacement, 2) the phong algorithm especially the boundary cases about total reflection. Also, the moving threshold of intersection points moving towards the intersection Normal.


**4. Shadow by Transmissive Objects**

The shadow reflection of transparent objects are implemented in this part.

![](http://i.imgur.com/FuBnwHk.png)

Still, I think there should be the same problem with the bugs stated above. 



**5. Texture Mapping**

We mapped some texture onto the scene according to some rule in UV mapping.

![](http://i.imgur.com/M6frlGA.png)

![](http://i.imgur.com/PmJ43W8.png)

Still, the shadow and Phong transmission problem will affect the whole scene. The problem at present is that, there is something wrong with the UV mapping of my triangle, and I have debugged to see if there is anything wrong with the UV mapping. However, it seems that it is the problem of calculating the intersection point, but I did not find anything wrong with my last assignment. So this problem is still unresolved.