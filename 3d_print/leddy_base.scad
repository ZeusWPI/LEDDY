//#cube([31.82, 31.82, 7.13]);
base_width = 32.3;
base_height = 50.22;
hole_radius = 1.5;
d_edge_to_side_of_holes = 1.15;
d_edge_to_center_of_holes = d_edge_to_side_of_holes + hole_radius;
d_edge_to_start_of_cut_out = d_edge_to_center_of_holes + hole_radius + d_edge_to_side_of_holes;
cut_out_width = base_width-2*d_edge_to_start_of_cut_out;
cut_out_height = base_height-2*d_edge_to_start_of_cut_out;
pin_height = 6;

// the base for the 12 displays is split in 2 for printing

// first 6 displays
for(i = [0 : 5]) {
    translate([i*base_width, 0, 0]) {
        difference() {
            cube([base_width, base_height, 1]);
            translate([d_edge_to_start_of_cut_out, d_edge_to_start_of_cut_out, -1]) {cube([cut_out_width, cut_out_height, 3]);}
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

// last 6 displays
for(i = [6 : 11]) {
    translate([i*base_width, 0, 0]) {
        difference() {
            cube([base_width, base_height, 1]);
            translate([d_edge_to_start_of_cut_out, d_edge_to_start_of_cut_out, -1]) {cube([cut_out_width, cut_out_height, 3]);}
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

// clips to connect parts
error = 0.3; // dont make it perfectly fit, make it a bit bigger
clip_width = 2 + 2*d_edge_to_start_of_cut_out + error;
clip_height = 5;

#translate([6*base_width - d_edge_to_start_of_cut_out - 1 - error/2,30,-1]) {
    difference() {
        cube([clip_width, clip_height, 2]);
        translate([1,-1,1]) {cube([clip_width-2, clip_height+2, 2]);};
    }
};