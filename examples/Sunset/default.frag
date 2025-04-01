#version 410 core

in vec4 v_color;
in vec2 v_texCoords;
in float v_textureSlot;

layout(location = 0) out vec4 fragColor;

uniform vec2 u_resolution;
uniform float u_time;

#define SKY_COLOR vec3(195./255., 76./255., 68./255.)

#define SUN_COLOR  vec3(255./255., 254./255., 213./255.) * 1.2
#define SUN_RADIUS 0.02
#define SUN_OUT_COLOR vec3(241./255., 189./255., 62./255.)
#define SUN_OUT_RADIUS 0.035

#define FLOOR_COLOR vec3(29./255., 5./255., 19./255.)
#define TREE_COLOR_1 vec3(29./255., 5./255., 19./255.) * 1.3
#define TREE_COLOR_2 vec3(66./255., 16./255., 39./255.)
#define TREE_COLOR_3 vec3(116./255., 38./255., 51./255.)

float rand(float x){
    return fract(sin(x)*1546455.0);
}

float tree_rand(float x){
    return (rand(x) - .5) / 100.;
}

float tree_branch(vec2 pos, float seed){
    float tree = 0;

    vec2 off_in = vec2(.1 + tree_rand(seed * 334), 0 +  1.2*tree_rand(seed * 948));
    vec2 off_out = vec2(.2 + tree_rand(seed * 54135), -.05 +  1.2*tree_rand(seed * 4865));

    // positve tree
    vec2 circle = pos + off_in;
    float radius = circle.x * circle.x + circle.y * circle.y;
    tree = step(radius, .1);
    tree = max(tree, smoothstep(.1, radius, .1002));

    // negative
    vec2 circle2 = circle + off_out;
    radius = circle2.x * circle2.x + circle2.y * circle2.y;
    tree = min(tree, 1 -  step(radius, .13));
    tree = min(tree, 1 - smoothstep(.13, radius, .1303));

    tree = min(tree, step(pos.x, 0));
    tree = min(tree, step(pos.y, 0));

    return tree;
}

float tree(vec2 pos){
    float tree = 0;

    // translate to middle
    pos -= vec2(u_resolution.x / u_resolution.y / 2, 1);

    for(float i = 0; i < 5; i++){
        tree = max(tree, tree_branch(pos, i * 654));
        tree = max(tree, tree_branch(pos * vec2(-1, 1), i * 845));

        pos /= vec2(1.15, 1.17);
        pos += vec2(0, .05);
    }

    return tree;
}

vec3 tree_color(float seed, float y){
    vec3 color = TREE_COLOR_1;
    float random = rand(seed);
    color = mix(color, TREE_COLOR_2, step(random, .66 - 1/(9 - y)));
    color = mix(color, TREE_COLOR_3, step(random, .33 - 1/(9 - y)));

    return color;
}

void main() {
    vec2 position = gl_FragCoord.xy/u_resolution.xy;
    position.x *= u_resolution.x / u_resolution.y;

    vec3 color = SKY_COLOR;

    vec2 sun = position;
    sun -= vec2(u_resolution.x / u_resolution.y / 2, .75);

    float radius = sun.x * sun.x + sun.y * sun.y;
    color = mix(color, SUN_OUT_COLOR, smoothstep(SUN_RADIUS, radius, SUN_OUT_RADIUS));
    color = mix(color, SUN_COLOR, smoothstep(SUN_RADIUS, radius, SUN_RADIUS + 0.002));
    color = mix(color, SUN_COLOR, step(radius, SUN_RADIUS));

    color = mix(color, SKY_COLOR, smoothstep(0., position.y, 0.3));

    color = mix(color, FLOOR_COLOR, step(position.y, 0.1));
    color = mix(color, FLOOR_COLOR, smoothstep(0.1, position.y, 0.101));

    vec2 treePos = position;
    treePos.x += u_time * .00004;

    treePos.y -= .08;
    vec2 orignial = treePos;

    for(float y = 0; y < 8; y++) {
        treePos = orignial;

        treePos.y += y * .02;
        treePos *= 6;
        treePos.x += rand(y);
        float nonRep = floor(treePos.x + 546);
        treePos.x = fract(treePos.x);
        treePos.x += .4;

        float randOff = rand(nonRep * y) * .15;
        color = mix(color, tree_color(randOff, y), tree(treePos + vec2(randOff, 0)));
    }

    fragColor = vec4(color, 1.0);
}
