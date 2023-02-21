
nr_of_screens = 12;
screen_width = 32.3;
box_height = 85;
border_size = 10;
box_width = nr_of_screens*screen_width + 2*border_size;
d_top_of_screen_to_top_of_box = 20;
box_thickness = 30;
screen_border_height = 6; // the border inside the box that neatly fits around the led matrices
screen_border_thickness = 1;

translate([-box_width,0,0]) {
    // top of box and inner border
    translate([0,0,box_thickness]) {
        difference() {
            union() {
                cube([box_width, box_height, 1]);
                translate([border_size-screen_border_thickness, box_height-d_top_of_screen_to_top_of_box-screen_width-screen_border_thickness,-screen_border_height]) {
                    cube([nr_of_screens*screen_width + 2*screen_border_thickness, screen_width + 2*screen_border_thickness, screen_border_height]);};
            }
            translate([border_size, box_height-d_top_of_screen_to_top_of_box-screen_width, -1-screen_border_height]) {
                cube([screen_width*nr_of_screens,screen_width, screen_border_height+3]);
            };
        }
    }
    // walls
    difference([]) {
        cube([box_width, box_height, box_thickness]);
        translate([screen_border_thickness,screen_border_thickness,-1]) {cube([box_width-2*screen_border_thickness, box_height-2*screen_border_thickness, box_thickness+2]);}
        
        // holes
        hole_thickness = 3;
        nr_of_holes = 6;
        holes_spacing = (nr_of_screens*screen_width)/nr_of_holes;
        for(i = [0:nr_of_holes-1]) {
            #translate([border_size+holes_spacing/2 + i*holes_spacing - hole_thickness/2,-1,2]) {cube([hole_thickness, box_height+2, hole_thickness]);};
        }
    }
    
};

// pins
pin_error=0.15;
module pin() {
    pin_thickness = hole_thickness-pin_error;
    cube([pin_thickness, box_height, pin_thickness]);
    translate([-1,-1,0]) {cube([pin_thickness+2, 1, pin_thickness+1]);};
}

