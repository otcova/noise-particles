let dots;
let speed = .3;
let stepSize = 0.005;
let dotSize = 4;

let pthreads = false;


function Setup() {
    p.createCanvas(p.windowWidth, p.windowHeight);

    dots = createSharedArray(Math.min(700000, parseInt(p.windowWidth * p.windowHeight * 0.08)) * 2 * dotSize, "f32");
    if (pthreads) Module.cppStartThread(dots.length, dots.byteOffset, p.width, p.height, stepSize, speed);
    else Module._cppSetup(dots.length, dots.byteOffset, p.width, p.height);

    p.pixelDensity(1);
    p.frameRate(14);
}

function Update() {
    p.background(10, 10, 15);

    if (!pthreads)
        Module._cppUpdate(dots.length, dots.byteOffset, p.width, p.height,
            stepSize, speed, p.mouseX, p.mouseY, p.mousePressed ? 200 : 0);

    p.loadPixels();
    for (let i = 0; i < dots.length; i += dotSize) {
        let index = (parseInt(dots[i]) % p.width + parseInt(dots[i + 1]) * p.width) * 4;
        let r = p.pixels[index] - 0;
        let g = p.pixels[index + 1] - 0;
        let b = p.pixels[index + 2] - 5;

        r = r * r * .16;
        g = g * 10;
        b = b * 40;
        //r =  r*50;
        //g =  g*3;
        //b =  b;

        p.pixels[index] = r;
        p.pixels[index + 1] = g;
        p.pixels[index + 2] = b;
        p.pixels[index + 3] = 255;
    }
    p.updatePixels();


}

function MousePressed() {
    p.fullscreen(true);
}