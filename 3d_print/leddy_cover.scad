//#cube([31.82, 31.82, 7.13]);
base_width = 32.3;
base_height = 50.22;
hole_radius = 1.5;
d_edge_to_center_of_holes = 1.15 + hole_radius;
pin_height = 6;
#for(i = [0 : 1]) {
    translate([i*base_width, 0, 0]) {
difference() {
    cube([base_width, base_height, 1]);
    translate([5, 8, -1]) {cube([22, 34, 3]);}
}

// bl
translate([d_edge_to_center_of_holes, d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
// br
translate([base_width - d_edge_to_center_of_holes, d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
// tl
translate([d_edge_to_center_of_holes, base_height - d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
// tr
translate([base_width - d_edge_to_center_of_holes, base_height - d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
}
}

total_height=15.26;
total_length=74;
display_start_length_from_top = 12;
width_of_ports=13;