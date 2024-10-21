import sys

# modular exponentiation

def modexp(x, y, N):
    if y == 0:
        return 1
    z = modexp(x, y // 2, N)
    if y % 2 == 0:
        return z * z % N
    else:
        return x * z * z % N
    
def find_pq(N):
    for i in range(2, N):
        if N % i == 0:
            p = i
            q = N // i
            return p, q
        
def find_lambda_n(p, q):
    return (p-1) * (q-1)

def find_d(e, lambda_n):
    for d in range(2, lambda_n):
        if (d * e) % lambda_n == 1:
            return d
        
# convert a string to its ASCII representation
def str_to_ascii(s):
    return [ord(c) for c in s]

#convert the ASCII to a string
def ascii_to_str(ascii):
    return ''.join([chr(c) for c in ascii])

# read in the message from a file where the first line is the message and the second line is the public key (e, N)
def read_message(filename):
    with open(filename, 'r') as f:
        message = f.readline().strip()
        e, N = map(int, f.readline().split())
    return message, e, N



# use file from command line argument
if len(sys.argv) != 2:
    print("Usage: python3 rsa_encrypt.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

message, e, n = read_message(filename)

# find P and Q

p, q = find_pq(n)

# Print P and Q
print("Prime number P:", p)
print("Prime number q:", q)

# print public key
print("Public key (e):", e)

# find lambda(n)
lambda_n = find_lambda_n(p, q)


# find d
d = find_d(e, lambda_n)

# print private key
print("Private key:", d)

print("n: ", n)

print("Phi of n:", lambda_n)

# convert the message to ASCII
ascii_message = str_to_ascii(message)

print("Message in ASCII code: ", ascii_message)


# encrypt the message
encrypted_message = [modexp(c, e, n) for c in ascii_message]

print("\nEncrypted message: ", encrypted_message)




