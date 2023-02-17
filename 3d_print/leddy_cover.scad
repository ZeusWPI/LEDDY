//#cube([31.82, 31.82, 7.13]);
base_width = 32.3;
base_height = 50.22;
hole_radius = 1.5;
d_edge_to_center_of_holes = 1.15 + hole_radius;
pin_height = 6;


total_height=15.26;
total_length=74;
display_start_length_from_top = 12;
width_of_ports=13;


// cover
screen_side = base_width;
height_base_and_screen = 15.6;
cover_thickness = 1;
chip_part_height = 17.8;
side_edge_width = 10; 
top_edge_width = 10;
bottom_edge_width = chip_part_height + 1;
clip_width = 5;
top_of_screen_to_top_of_chip_board = 10.9;
chip_board_thickness = 1.5;

lip = 1;
clip_length = top_of_screen_to_top_of_chip_board-cover_thickness+chip_board_thickness+lip;
module bottom_clip() {    
    translate([-6*screen_side,0,-clip_length]) {
        cube([clip_width,1,clip_length]);
        translate([0,1,0]) {cube([clip_width,1,1]);};
    };
}

module top_clip() {
    
    translate([-6*screen_side,screen_side+chip_part_height+1,-clip_length]) {
        cube([clip_width,1,clip_length]);
        translate([0,-1,0]) {cube([5,1,1]);};
        translate([0,-1,1+chip_board_thickness + 0.5]) {cube([5,1,1]);};
    };
}

module stick() {
    cylinder_height = top_of_screen_to_top_of_chip_board-cover_thickness;
    translate([-(6*screen_side) + 7,5.5,-cylinder_height]) {cylinder(cylinder_height, 2, 2);};
    translate([-(6*screen_side) + screen_side - 7,5.5,-cylinder_height]) {cylinder(cylinder_height, 2, 2);};
}

module cover() {
    difference() {
        translate([-(6*screen_side + side_edge_width),0,0]) {cube([6*screen_side + side_edge_width, screen_side + top_edge_width + bottom_edge_width, cover_thickness]);};
        translate([-(6*screen_side), bottom_edge_width, -1]) {cube([6*screen_side + 1, screen_side, 3]);};
    }
    linear_extrude(cover_thickness) {polygon([[0, 0], [0, -40], [-100, -40], [-130, 0]]);};
    
    for(i = [0 : 5]) {
        translate([i*screen_side,0,0]) {
            translate([5,0,0]) {
                bottom_clip();
                top_clip();
            }
            translate([screen_side-clip_width-4,0,0]) {
                bottom_clip();
                top_clip();
            }
            stick();
        }
    }
}
//cover();
difference() {
    cover();
    translate([-screen_side*5,-50,-20]) {cube([1000,1000,1000]);};
}
//mirror([1,0,0]) {
//    cover();
//}