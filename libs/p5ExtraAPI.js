let p;

function startP5() {
    if (typeof p5 == "undefined") {
        setTimeout(startP5, 10)
    } else {
        const p5Body = (lp) => {
            lp.setup = function () {
                p = lp;
                Setup();
            };
            lp.draw = function () {
                Update();
            };
            lp.mousePressed = function () {
                MousePressed();
            };
            lp.windowResized = function () {
                lp.resizeCanvas(lp.windowWidth, lp.windowHeight)
            };
        }
        new p5(p5Body);
    }
};

Module.onRuntimeInitialized = startP5;
//startP5();
