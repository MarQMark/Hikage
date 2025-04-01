import hikage
import numpy as np

pos = np.array([0.0, 0.0, 0.0])
vel = np.array([0.0, 0.0, 0.0])

speed = 5

wPressed = False
sPressed = False
dPressed = False
aPressed = False
qPressed = False
ePressed = False

cLock = False

yaw = 90
pitch = 0

def init():
    pass
    
def get_camera_basis(yaw, pitch):
    yaw_rad = np.radians(yaw)
    pitch_rad = np.radians(pitch)

    front = np.array([
        np.cos(yaw_rad) * np.cos(pitch_rad),
        np.sin(pitch_rad),
        np.sin(yaw_rad) * np.cos(pitch_rad)
    ])
    front /= np.linalg.norm(front)

    right = np.cross(front, np.array([0.0, 1.0, 0.0]))
    right /= np.linalg.norm(right)
    up = np.cross(right, front)
    up /= np.linalg.norm(up)

    return front, right, up

def update():
    global pos
    global vel

    global yaw
    global pitch

    global speed

    global wPressed
    global sPressed
    global dPressed
    global aPressed
    global qPressed
    global ePressed

    global cLock

    dt = hikage.getDT()
    front, right, up = get_camera_basis(yaw, pitch)


    if(wPressed):
        vel += front
        vel /= np.linalg.norm(vel)
        vel *= speed
    if(sPressed):
        vel -= front
        vel /= np.linalg.norm(vel)
        vel *= speed
    if(dPressed):
        vel += right
        vel /= np.linalg.norm(vel)
        vel *= speed
    if(aPressed):
        vel -= right
        vel /= np.linalg.norm(vel)
        vel *= speed
    if(qPressed):
        vel += np.array([0.0, 1.0, 0.0]) * speed
    if(ePressed):
        vel -= np.array([0.0, 1.0, 0.0]) * speed

    vel *= .8
    pos += vel * dt

    hikage.setUniform3f("u_cam_pos", pos[0], pos[1], pos[2])
    hikage.setUniform3f("u_cam_dir", front[0], front[1], front[2])

def clbkKey(key, scancode, action, mods):
    global wPressed
    global sPressed
    global dPressed
    global aPressed
    global qPressed
    global ePressed

    global cLock

    global speed

    if(key == hikage.KEY_W and action == hikage.KEY_PRESS):
        wPressed = True
    elif(key == hikage.KEY_W and action == hikage.KEY_RELEASE):
        wPressed = False

    if(key == hikage.KEY_S and action == hikage.KEY_PRESS):
        sPressed = True
    elif(key == hikage.KEY_S and action == hikage.KEY_RELEASE):
        sPressed = False

    if(key == hikage.KEY_D and action == hikage.KEY_PRESS):
        dPressed = True
    elif(key == hikage.KEY_D and action == hikage.KEY_RELEASE):
        dPressed = False

    if(key == hikage.KEY_A and action == hikage.KEY_PRESS):
        aPressed = True
    elif(key == hikage.KEY_A and action == hikage.KEY_RELEASE):
        aPressed = False

    if(key == hikage.KEY_Q and action == hikage.KEY_PRESS):
        qPressed = True
    elif(key == hikage.KEY_Q and action == hikage.KEY_RELEASE):
        qPressed = False

    if(key == hikage.KEY_E and action == hikage.KEY_PRESS):
        ePressed = True
    elif(key == hikage.KEY_E and action == hikage.KEY_RELEASE):
        ePressed = False

    if(key == hikage.KEY_ESCAPE and action == hikage.KEY_PRESS):
        hikage.setCursorVisibility(True)
        cLock = False

    if(key == hikage.KEY_LEFT_SHIFT):
        if(action == hikage.KEY_PRESS):
            speed = 25
        elif(action == hikage.KEY_RELEASE):
            speed = 5

def clbkMouseBtn(btn, action, mods):
    global cLock

    if(btn == hikage.MOUSE_BUTTON_2 and action == hikage.KEY_PRESS):
        hikage.setCursorVisibility(cLock)
        if(cLock):
            cLock = False
        else:
            cLock = True

def clbkMousePos(x, y):
    global yaw
    global pitch
    global cLock

    if(cLock == False):
        return

    w = hikage.getResolutionX() / 2
    h = hikage.getWindowH() / 2
    hikage.setCursorPos(w, h)

    yaw += (x - w) / 20
    pitch += (h - y) / 20


