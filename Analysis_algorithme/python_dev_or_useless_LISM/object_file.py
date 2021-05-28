

class Object_simulated:
    def __init__(self, object_number):
        self.object_number = object_number
        self.obj_n_bad = []
        self.obj_n_good = []
        self.obj_dl_max = []
        self.dx = []
        self.dy = []
        self.angle = []

    def __repr__(self):
        return 'Object_simulated(object_number=%s, obj_n_bad=%s, obj_n_good=%s, obj_dl_max=%s, dx=%s, dy=%s, angle=%s)' % (self.object_number, self.obj_n_bad, self.obj_n_good, self.obj_dl_max, self.dx, self.dy, self.angle)

    def set_parameters(self, object_number, obj_n_bad, obj_n_good, obj_dl_max, dx, dy, angle):
        self.object_number = object_number
        self.obj_n_bad = obj_n_bad
        self.obj_n_good = obj_n_good
        self.obj_dl_max = obj_dl_max
        self.dx = dx
        self.dy = dy
        self.angle = angle


if __name__ == "__main__":
    # execute only if run as a script
    object_number1 = Object_simulated(4)
    object_number1.set_parameters(4, 155, 10, 5, 41, 51, 2)
    print(object_number1)
