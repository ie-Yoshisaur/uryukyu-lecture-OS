import argparse
import secrets
import string

def generate_password(password_length):
    valid_chars_for_password = string.ascii_letters + string.digits + string.punctuation
    password = ''.join(secrets.choice(valid_chars_for_password) for _ in range(password_length))
    print(password)

if __name__=='__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("password_length")
    args = parser.parse_args()
    password_length = int(args.password_length)
    generate_password(password_length)
