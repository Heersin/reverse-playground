very_secure_secret = "7=28L(6`4_>b0E@0A=cJ0qC@PN"
alphabet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"+ \
            "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

def decode(secret):
    rotate_const = 47
    decoded = ""

    for c in secret:
        index = alphabet.find(c)
        original_index = (index + rotate_const) % len(alphabet)
        decoded = decoded + alphabet[original_index]

    print(decoded)

def choose_greatest():
    print("AI-based IQ Test")
    user_value_1 = input("What's your lucky number ")
    user_value_2 = input("What's your birthday ")
    greatest_value = user_value_1

    if user_value_1 > user_value_2:
        greatest_value = user_value_1
    elif user_value_1 < user_value_2:
        greatest_value = user_value_2

    print("You are a talent!")

if __name__ == "__main__":
    choose_greatest()
