import time

def init_2d_list(dim1, dim2, val):
    return [[val for j in range(dim2)] for i in range(dim1)]


def add_to_dict(d, key, val):
    if key in d:
        return False
    else:
        d[key] = val
        return True


class TuringMachine():
    def __init__(self, input_file_name):
        self.input_file_name = input_file_name
        self.num_states = 0
        self.num_alphs = 0
        self.ptr = 0
        self.accept_state = []
        self.state_id = {}
        self.alph_id = {}
        self.state_table = []
        self.dir_table = []
        self.write_table = []
        self.tape = ""

    def displayTape(self, present_state_id):
        flog = open("log.txt", 'a')

        print('\r', self.tape[0:self.ptr], sep='', end='')
        print('[', self.tape[self.ptr], ']', sep='', end='')
        print(self.tape[self.ptr+1:], sep='', end='')

        flog.write(self.tape[0:self.ptr])
        flog.write('[' + self.tape[self.ptr] + ']')
        flog.write(self.tape[self.ptr+1:])

        # get name of the present state
        state_name = ''
        for key, val in self.state_id.items():
            if val == present_state_id:
                state_name = key
        print('   ', state_name, '     ', sep='', end='')
        flog.write('   ' + state_name + '\n')

        time.sleep(0.2)
        flog.close()

    def parseFile(self):
        fin = open(self.input_file_name, 'r')
        self.tape = fin.readline().strip()
        for line in fin:
            strs = line.split()
            if len(strs) == 0:
                continue
            if strs[0] == "accept":
                for present_state in strs[1:]:
                    self.accept_state.append(self.state_id[present_state])
                continue
            else:
                present_state = strs.pop(0)
            read_char = strs.pop(0)
            write_char = strs.pop(0)
            direction = strs.pop(0)
            next_state = strs.pop(0)
            if add_to_dict(self.state_id, present_state, self.num_states):
                self.num_states = self.num_states + 1
            if add_to_dict(self.state_id, next_state, self.num_states):
                self.num_states = self.num_states + 1
            if add_to_dict(self.alph_id, read_char, self.num_alphs):
                self.num_alphs = self.num_alphs + 1
            if add_to_dict(self.alph_id, write_char, self.num_alphs):
                self.num_alphs = self.num_alphs + 1
            self.state_table = init_2d_list(self.num_states, self.num_alphs, -1)
            self.write_table = init_2d_list(self.num_states, self.num_alphs, '_')
            self.dir_table = init_2d_list(self.num_states, self.num_alphs, 0)
        fin.close()

    def makeTransitionTables(self):
        fin = open(self.input_file_name, 'r')
        line = fin.readline()
        for line in fin:
            strs = line.split()
            if len(strs) == 0:
                continue
            if strs[0] == "accept":
                continue
            else:
                present_state = strs.pop(0)
            read_char = strs.pop(0)
            write_char = strs.pop(0)
            direction = strs.pop(0)
            next_state = strs.pop(0)

            # find id's
            present_state_id = self.state_id[present_state]
            next_state_id = self.state_id[next_state]
            read_char_id = self.alph_id[read_char]
            if direction == "l":
                dir_id = -1
            elif direction == "r":
                dir_id = +1
            else:
                dir_id = 0

            # fill transition tables with id's
            self.state_table[present_state_id][read_char_id] = next_state_id
            self.write_table[present_state_id][read_char_id] = write_char
            self.dir_table[present_state_id][read_char_id] = dir_id

        fin.close()

    def turingSimulator(self):
        f = open("log.txt", 'w')
        f.close()

        present_state_id = 0
        self.displayTape(present_state_id)
        while True:
            read_char_id = self.alph_id[self.tape[self.ptr]]
            dir_id = self.dir_table[present_state_id][read_char_id]
            self.tape = self.tape[0:self.ptr] + self.write_table[present_state_id][read_char_id] + self.tape[self.ptr+1:]
            present_state_id = self.state_table[present_state_id][read_char_id]

            if present_state_id == -1:
                if len(self.accept_state) > 0:
                    print("\n", "> rejected", sep='', end='')
                else:
                    print("\n", "> halted", sep='', end='')
                break

            if present_state_id in self.accept_state:
                print("\n", "> accepted", sep='', end='')
                break

            self.ptr += dir_id
            if self.ptr < 0:
                self.tape = "_" + self.tape
                self.ptr = 0
            if self.ptr >= len(self.tape):
                self.tape = self.tape + "_"
            self.displayTape(present_state_id)

