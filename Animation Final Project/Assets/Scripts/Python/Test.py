import UnityEngine;
import pronouncing;
import re;

# Get the text from the phoneme input field
input_string = UnityEngine.GameObject.Find("InputField_GeneratePhonemes").GetComponent("TMP_InputField").text

# Validate that input is not empty
if len(input_string) > 0:
    # Strip punctation from string
    input_string = re.sub("[^\w\s]", "", input_string)
    # Validate that input is not just punctuation
    if len(input_string) > 0:
        # Split the input string into individual words
        input_string = input_string.split()
        print(input_string)

        # Store the number of words for iteration syncing with audio later
        len_input_string = len(input_string)
        #print(len_input_string)

        input_phonemes = []

        # Iterate through the words in the divided string
        for word in input_string:
            pronunciation_list = pronouncing.phones_for_word(word)
            # Verify that the phonemic data exists for the word
            if len(pronunciation_list) > 0:
                # Get the first pronunciation and split it into a list of phonemes
                phonemes = pronunciation_list[0].split()
                # Append the phoneme list to the phrase phoneme list
                input_phonemes.append(phonemes)
            else:
                # Clear the phrase phoneme list
                input_phonemes.clear()
                print("ERROR: UNRECOGNIZED WORD")
                break

        # Check if the phrase has been converted to phonemes successfully, and transfer the output
        if len(input_phonemes) > 0:
            print(input_phonemes)


    else:
        print("ERROR: NO VALID CHARACTERS")
else:
    print("ERROR: EMPTY STRING")


