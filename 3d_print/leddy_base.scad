//#cube([31.82, 31.82, 7.13]);
base_width = 32.3;
base_height = 50.22;
hole_radius = 1.5;
d_edge_to_center_of_holes = 1.15 + hole_radius;
pin_height = 6;
for(i = [0 : 12]) {
    translate([i*base_width, 0, 0]) {
        difference() {
            cube([base_width, base_height, 1]);
            translate([5, 8, -1]) {cube([22, 34, 3]);}
        }

        // bottom left
        translate([d_edge_to_center_of_holes, d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
        // bottom right
        translate([base_width - d_edge_to_center_of_holes, d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
        // top left
        translate([d_edge_to_center_of_holes, base_height - d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
        // top right
        translate([base_width - d_edge_to_center_of_holes, base_height - d_edge_to_center_of_holes, 0]) {cylinder(pin_height, hole_radius, hole_radius, $fn=50);}
    }
}