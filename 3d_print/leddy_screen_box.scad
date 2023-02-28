
nr_of_screens = 12;
screen_width = 32.3;
box_height = 85;
border_size = 10;
box_width = nr_of_screens*screen_width + 2*border_size;
d_top_of_screen_to_top_of_box = 20;
box_thickness = 31;
screen_border_height = 5; // the border inside the box that neatly fits around the led matrices
screen_border_thickness = 1;

hole_thickness = 3;
nr_of_holes = 6;
holes_spacing = (nr_of_screens*screen_width)/nr_of_holes;
d_holes_to_bottom = 2;

module box() {
    translate([-box_width,0,0]) {
        // top of box and inner border
        translate([0,0,box_thickness-1]) {
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
            for(i = [0:nr_of_holes-1]) {
                translate([border_size+holes_spacing/2 + i*holes_spacing - hole_thickness/2,-1,d_holes_to_bottom]) {cube([hole_thickness, box_height+2, hole_thickness]);};
            }
        }
        
    };
}

screen_and_pcb_thickness = 12.5;
cover_thickness = 1;
cover_support_pins_height = box_thickness - d_holes_to_bottom - hole_thickness - screen_and_pcb_thickness - cover_thickness +1; // +1 om een beetje spanning te geven
d_cover_sopport_pin_from_top = d_top_of_screen_to_top_of_box + 25;
module cover() {
    translate([-box_width+1,1,d_holes_to_bottom+hole_thickness]) {
        cube([box_width-2, box_height-2, cover_thickness]);
        for(i = [0:nr_of_screens-1]) {
            translate([border_size+screen_width/2 + i*screen_width,box_height-d_cover_sopport_pin_from_top,cover_thickness]) {
                cylinder(cover_support_pins_height,3,3,$f=10);
            }
        }
    };
    
}

// pins
pin_error=0.15;
module pin() {
    pin_thickness = hole_thickness-pin_error;
    difference () {
        cube([pin_thickness, box_height+2, pin_thickness]);
        translate([pin_thickness/2,box_height,-1]) { cylinder(h=pin_thickness+2,r1=0.5,r2=0.5,$fn=10);}
    }
    translate([-1,-1,0]) {cube([pin_thickness+2, 1, pin_thickness+1]);};
}

// split the box in 2
d_connector_to_bottom = d_holes_to_bottom+hole_thickness;
connector_thickness = box_thickness-d_connector_to_bottom-1;
connector_width = 5;
module left_box() {
    difference() {
        box();
        translate([-box_width/2,-1,-1]) cube([box_width/2+1,box_height+2,box_thickness+2]);
    }
    
    // connector
    #translate([-box_width/2-connector_width,1,d_connector_to_bottom]) {
        cube([connector_width,connector_width,connector_thickness]);
        translate([0,box_height-connector_width-2,0]) {cube([connector_width,connector_width,connector_thickness]);}
    }
}
//left_box();
connector_error = 0.15; // distance between 2 pieces of the connector when they fit together
module right_box() {
    difference() {
        box();
        translate([-box_width-1,-1,-1]) cube([box_width/2+1,box_height+2,box_thickness+2]);
    }
    
    // connector
    cube_width = 16;
    cube_height = 10;
    difference() {
        translate([-box_width/2-cube_width/2,1,d_connector_to_bottom]) cube([cube_width,cube_height,connector_thickness]);
        translate([-box_width/2-connector_width-connector_error,0,d_connector_to_bottom-1]) {
            cube([connector_width+2*connector_error,connector_width+connector_error+1,connector_thickness+2]);
        }
    };
    difference() {
        translate([-box_width/2-cube_width/2,box_height-cube_height-1,d_connector_to_bottom]) cube([cube_width,10,connector_thickness]);
        translate([-box_width/2-connector_width-connector_error,box_height-connector_width-1-connector_error,d_connector_to_bottom-1]) {
            cube([connector_width+2*connector_error,connector_width+connector_error+1,connector_thickness+2]);
        }
    };
}

left_box();
right_box();
