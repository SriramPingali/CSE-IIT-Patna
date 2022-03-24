// writing a computer program to deduce the total number of atoms and surface atoms 
// for different shell of cuboctahedral/spherical shape.
// Plot % of atoms in bulk/surface versus particle size.

function cubo_octahedral_total(k) {
    ans = (10 * (k ** 3) + 15 * (k ** 2) + 11 * k + 3) / 3;
    return ans;
}

function cubo_octahedral_surface(k) {
    ans = 10 * (k ** 2) + 2;
    return ans;
}

function spherical_total(k) {
    ans = (10 * (k ** 3) - 15 * (k ** 2) + 11 * k - 3) / 3;
    return ans;
}

function spherical_surface(k) {
    ans = 10 * (k ** 2) - 20 * k + 12;
    return ans;
}

function autoSize() {
    var application;
    var selectedApp = document.querySelector('input[name="application"]:checked');
    if (selectedApp != null) {
        application = selectedApp.value;
    }
    console.log('application=' + application);

    switch (application) {
        case 'Optical':
            document.getElementById("min_particle_size").value = "40";
            document.getElementById("max_particle_size").value = "100";
            console.log("Change Values");
            break;
        case 'Electrical':
            document.getElementById("min_particle_size").value = "10";
            document.getElementById("max_particle_size").value = "20";
            break;
        case 'Magnetic':
            document.getElementById("min_particle_size").value = "1";
            document.getElementById("max_particle_size").value = "10";
            break;
        case 'Strength':
            document.getElementById("min_particle_size").value = "1";
            document.getElementById("max_particle_size").value = "50";
            break;
        case 'None':
            document.getElementById("min_particle_size").value = "1";
            document.getElementById("max_particle_size").value = "50";
            break;
        default:
            break;
    }
}

window.onload = function() {
    var sizes_of_particle = []
    for (var i = 1; i <= 50; i++) {
        sizes_of_particle.push(i);
    }

    var dummy_percent = []
    for (var i = 0; i <= 100; i++) {
        dummy_percent.push(i);
    }

    var layout = {
        autosize: false,
        width: 650,
        margin: {
            l: 50,
            r: 50,
            b: 100,
            t: 100
        },
        xaxis: {
            title: 'Particle Size',
            range: sizes_of_particle
        },
        yaxis: {
            title: '% of atoms in Bulk/Surface',
            range: dummy_percent
        }
    };

    var graph_bulk = {
        x: sizes_of_particle,
        y: [],
        name: 'Bulk Atoms',
        type: 'scatter'
    };

    var graph_surface = {
        x: sizes_of_particle,
        y: [],
        name: 'Surface Atoms',
        type: 'scatter'
    };

    var data = [graph_bulk, graph_surface];
    // document.getElementById('results').innerHTML = "For <b>" + shape + "</b> shape, <b>% of atoms in Bulk/Surface vs. Particle Size</b> for "+shape+" shape";
    Plotly.newPlot('finalPlot', data, layout);
}

function draw() {
    var shapes = document.getElementsByName('shape');
    console.log('shapes=' + shapes)
    var shape;

    var selectedShape = document.querySelector('input[name="shape"]:checked');
    if (selectedShape != null) {
        shape = selectedShape.value;
    } else {
        alert('Select one of the shapes');
        return;
    }
    console.log('shape=' + shape);

    var min_particle_size = document.getElementById("min_particle_size").value;
    console.log('min_particle_size=' + min_particle_size);
    if (min_particle_size != "") {
        min_particle_size = parseInt(min_particle_size);
    } else {
        min_particle_size = 1;
    }
    console.log('final min_particle_size=' + min_particle_size);

    var max_particle_size = document.getElementById("max_particle_size").value;
    console.log('max_particle_size=' + max_particle_size);
    if (max_particle_size != "") {
        max_particle_size = parseInt(max_particle_size);
    } else {
        max_particle_size = 50;
    }
    console.log('final max_particle_size=' + max_particle_size);

    sizes_of_particle = []
    for (var i = min_particle_size; i <= max_particle_size; i++) {
        sizes_of_particle.push(i);
    }

    atoms_surface = []
    atoms_bulk = []
    if (shape == 'Cuboctahedral') {
        sizes_of_particle.forEach(function(particle_size, index) {
            x = (cubo_octahedral_surface(particle_size) / cubo_octahedral_total(particle_size)) * 100;
            atoms_surface.push(x);
            atoms_bulk.push(100 - x);
        });
    } else if (shape == 'Spherical') {
        sizes_of_particle.forEach(function(particle_size, index) {
            x = (spherical_surface(particle_size) / spherical_total(particle_size)) * 100;
            if (x > 100) x = 100;
            atoms_surface.push(x);
            atoms_bulk.push(100 - x);
        });
    }

    var graph_bulk = {
        x: sizes_of_particle,
        y: atoms_bulk,
        name: 'Bulk Atoms',
        type: 'scatter'
    };

    var graph_surface = {
        x: sizes_of_particle,
        y: atoms_surface,
        name: 'Surface Atoms',
        type: 'scatter'
    };

    var layout = {
        autosize: false,
        width: 650,
        // height: 700,
        margin: {
            l: 50,
            r: 50,
            b: 100,
            t: 100
        },
        xaxis: {
            title: 'Particle Size'
        },
        yaxis: {
            title: '% of atoms in Bulk/Surface'
        },
        title: '<b>% of atoms in Bulk/Surface vs. Particle Size</b> for '+shape+' shape'
    };

    var data = [graph_bulk, graph_surface];
    // document.getElementById('results').innerHTML = "For <b>" + shape + "</b> shape, <b>% of atoms in Bulk/Surface vs. Particle Size</b> for "+shape+" shape";
    Plotly.newPlot('finalPlot', data, layout);
    document.getElementById('finalPlot').scrollIntoView();
}