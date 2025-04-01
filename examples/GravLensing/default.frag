#version 430 core

in vec4 v_color;
in vec3 v_normal;
in vec3 v_fragPos;

uniform vec2 u_resolution; // Screen resolution (width, height)
uniform float u_time;      // Time in milli seconds
uniform vec2 u_mouse;

/*
 *  Textures need to be added for both samplers
 *  in order for the plantes to display correctly
 */

uniform sampler2D u_sampler;
uniform sampler2D u_sampler2;

uniform vec3 u_cam_pos;
uniform vec3 u_cam_dir;

layout(location = 0) out vec4 fragColor;


#define GRAV_CONST 6.67430e-11
#define SPEED_OF_LIGHT 3e8

#define MAX_DEPTH 333.f
#define STEP_SIZE 10e2f


struct Body{
    vec3 c;     // center
    float r;    // radius
    float o;    // orbit radius
    float m;    // mass
};

Body hole = Body(
    vec3(0.0, 0.0, 60000.0),
    1,
    0,
    5e30
);

Body planet = Body(
    vec3(0.0),
    3000,
    20000,
    0
);

Body planet2 = Body(
    vec3(0.0),
    6000,
    35000,
    0
);


vec3 line_sphere_int(vec3 p0, vec3 v, vec3 cs, float r){
    vec3 p0c = p0 - cs;
    float a = dot(v, v);
    float b = 2 * dot(p0c, v);
    float c = dot(p0c, p0c) - r * r;

    float d = b * b - 4 * a * c;
    if (d < 0)
        return vec3(0);  // No intersection

    float sqrt_d = sqrt(d);
    float t1 = (-b + sqrt_d) / (2 * a);
    float t2 = (-b - sqrt_d) / (2 * a);

    float t = min(t1, t2);

    return (p0 + v * t);
}

float rand() {
    return step(0.998, fract(sin(gl_FragCoord.x * 0.9898 + gl_FragCoord.y * 0.233) * 43758.5453));
}

vec3 update_dir(vec3 p, vec3 v, Body b){
    vec3 r_vec = b.c - p;
    float r = length(r_vec);
    if(r == 0)
    return vec3(0.0);

    vec3 g = r_vec / r;
    float d_alpha = (4 * GRAV_CONST * b.m) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT * r) * (STEP_SIZE / r);
    return v += d_alpha * g;
}


bool coll_sphere(vec3 p, vec3 v, Body b){
    vec3 a = (p + v) - b.c;
    return (a.x * a.x + a.y * a.y + a.z * a.z) < (b.r*b.r);
}

bool coll_planet(vec3 p, vec3 v, Body b, sampler2D s, out vec3 c){
    if(coll_sphere(p, v, b)){
        vec3 i = line_sphere_int(p, v, b.c, b.r);
        i = (b.c - i) / (b.r);// + vec3(.5);
        i = vec3(
            i.x * sin(u_time / 400) + i.z * cos(u_time / 400),
            i.y,
            -i.x * cos(u_time / 400) + i.z * sin(u_time / 400)
        );
        float u = atan(i.x, i.z) / (2.0f * 3.141592) + 0.5f;
        float v = asin(i.y) / 3.141592 + 0.5f;
        c = vec3(texture(s, vec2(u, v)));
        return true;
    }

    return false;
}


vec3 step(vec3 p, vec3 v){
    for(float i = 0; i < MAX_DEPTH; i++){

        v = update_dir(p, v, hole);

        vec3 v1 = (v * STEP_SIZE);
        vec3 p1 = p + v1;

        if(coll_sphere(p, v1, hole))
            return vec3(0);

        vec3 c = vec3(0);
        if(coll_planet(p, v1, planet, u_sampler, c))
            return c;
        if(coll_planet(p, v1, planet2, u_sampler2, c))
            return c;

        p = p1;
    }

    return mix(vec3(0.02), vec3(1.0), rand());
}

void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution.xy) * 2.0 - 1.0;
    uv.x *= u_resolution.x / u_resolution.y;

    // update planet position
    //planet.c =  vec3(cos(u_time / 1000) * planet.o, 0.0, sin(u_time / 1000) * planet.o + hole.c.z);
    hole.r = (2 * GRAV_CONST * hole.m) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);

    planet2.c =  vec3(cos(u_time / 700) * planet2.o, 0.0, sin(u_time / 700) * planet2.o + hole.c.z);

    //vec3 ro = vec3(60000 * u_mouse.y / u_resolution.y, 0.0, 60000 * u_mouse.x / u_resolution.x); // Ray origin
    vec3 ro = vec3(0.0); // Ray origin
    vec3 rd = normalize(vec3(uv, 1.0)); // Ray direction

    float fx = tan(90 / 2.0) / u_resolution.x;
    vec2 d = (2.0 * gl_FragCoord.xy - u_resolution) * fx;
    vec3 right = normalize(cross(u_cam_dir, vec3(0, 1, 0)));  // Right vector
    vec3 up = normalize(cross(right, u_cam_dir));     // Corrected Up vector
    rd = normalize(d.x * right + d.y * up + u_cam_dir);

    //rd = normalize(vec3(d, 1.0));
    ro = u_cam_pos;

    vec3 color = step(ro, rd);

    color = mix(color, vec3(1), coll_sphere(ro, vec3(0), hole));

    fragColor = vec4(color, 1.0);
}
