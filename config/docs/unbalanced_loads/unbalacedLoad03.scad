// template.scad


$fn=60;

// variables

//// variable 1
//d1 = 10;    // [1:1:100]
//// variable 2
//l1 = 65;    // [5:1:80]
//// variable 3
//l2 = 28;    // [1:1:69]

nutD = 11.2;
nutT = 5;

thick = 18;
nutSpace = 13;

diam = 40;

main();


module main()
{
    
//    translate([50,0,0])cube_r(30,20,10,2);
//    
//    translate([0,50,0])rotate([90,0,0])rectTriangle(10,20,5);
//    
//    linear_extrude(12)translate([10,10,0])import("BFKDO.svg");
//    
//    translate([50,50,0])rotate([0,0,0])linear_extrude(height = 0.5)text("(c) ChB 2/2021",size = 5);
//    
//    translate([100,0,0])cylinder_r(40,12,8,3,2);
    
    difference(){
        plus();
        minus();

    }
}


module plus()
{   
//    translate([0,0,15])rotate([0,0,0])cube([l2,l2,2*l2],true);
    translate([0,0,0])rotate([0,0,0])cylinder(thick,diam/2,diam/2);
    
    translate([diam/2,0,0])rotate([0,0,0])cylinder(thick,2,2);
    translate([-diam/2,0,0])rotate([0,0,0])cylinder(thick,2,2);
}

module minus()
{
    translate([0,0,0])rotate([0,0,0])cylinder(14,8.10/2,7.80/2);
    
    for (a =[0:60:360])
    { 
    
        rotate([0,0,a])translate([nutSpace,0,thick-nutT])cylinder($fn=6,nutT,nutD/2,nutD/2);
        rotate([0,0,a])translate([nutSpace,0,0])cylinder(thick,7/2,7/2);
        
    }

   
}

module cube_r(x,y,z,r)
{
    hull()
    {
        translate([r,r,r])cube([x-2*r,y-2*r,z-2*r]);
        translate([r,r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        translate([r,y-r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        translate([x-r,r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        translate([x-r,y-r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        
        translate([r,r,r])rotate([0,90,0])cylinder(x-2*r,r,r);
        translate([r,y-r,r])rotate([0,90,0])cylinder(x-2*r,r,r);
        translate([r,y-r,z-r])rotate([0,90,0])cylinder(x-2*r,r,r);
        translate([r,r,z-r])rotate([0,90,0])cylinder(x-2*r,r,r);
        
        translate([x-r,y-r,r])rotate([90,0,0])cylinder(y-2*r,r,r);
        translate([r,y-r,r])rotate([90,0,0])cylinder(y-2*r,r,r);
        translate([r,y-r,z-r])rotate([90,0,0])cylinder(y-2*r,r,r);
        translate([x-r,y-r,z-r])rotate([90,0,0])cylinder(y-2*r,r,r);
        
        translate([r,r,r])sphere(r = r);
        translate([x-r,r,r])sphere(r = r);
        translate([r,y-r,r])sphere(r = r);
        translate([x-r,y-r,r])sphere(r = r);
        
        translate([r,r,z-r])sphere(r = r);
        translate([x-r,r,z-r])sphere(r = r);
        translate([r,y-r,z-r])sphere(r = r);
        translate([x-r,y-r,z-r])sphere(r = r);
    }
}

module cylinder_r(h,r1,r2,r3,r4)
{
    hull()
    {
        translate([0,0,r3])cylinder(h-r3-r4,r1-r3,r2-r4);
        
        translate([0,0,r3])rotate_extrude(convexity = 10, $fn = 100)
            translate([r1, 0, 0])circle(r = r3, $fn = 100);
        
        translate([0,0,h-r4])rotate_extrude(convexity = 10, $fn = 100)
            translate([r2, 0, 0])circle(r = r4, $fn = 100);
    }
    
}

module rectTriangle(a,b,h)
{
linear_extrude(height = h, center = false, convexity = 10, twist = 0)
    polygon(points=[[0,0],[a,0],[0,b]]);   
}


