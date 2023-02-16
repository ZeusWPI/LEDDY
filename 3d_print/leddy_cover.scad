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
height_widht_base = 15.6;
chip_part_height = 18.3;
side_edge_width = 10; 
top_edge_width = 10;
bottom_edge_width = chip_part_height + 2;

module cover() {
    difference() {
        cube([6*screen_side + side_edge_width, screen_side + top_edge_width + bottom_edge_width, 1]);
        translate([-1, bottom_edge_width, -1]) {cube([6*screen_side + 1, screen_side, 3]);};
    }
    #polygon([[0, 0], [0, -40], [-100, -40], [-130, 0]]);
}
cover();
mirror([1,0,0]) {
    cover();
}