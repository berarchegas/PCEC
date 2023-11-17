import os
import subprocess

# Define the main folder containing subfolders with graph files
main_folder = os.path.join(os.path.dirname(__file__), 'andreasCisiRamos')

# Define the path to your C++ executable
cpp_executable = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'src', 'main', 'main.exe')

# Define the output file
output_file = os.path.join(os.path.dirname(__file__), 'brunoFreitas', 'output.txt')

# Function to generate paths of size 3 and append them to the output file
def generate_paths(input_graph_file):
    with open(output_file, 'a') as output:
        subprocess.run([cpp_executable], stdin=open(input_graph_file), stdout=output, text=True, shell=True)

# Walk through the main folder and its subfolders
for root, dirs, files in os.walk(main_folder):
    for file_name in files:
        if file_name.endswith('.txt'):
            input_graph_path = os.path.join(root, file_name)

            with open(output_file, 'a') as output:
                output.write(file_name + ': ')
            # Process the graph file and append paths to the output file
            generate_paths(input_graph_path)

print("Processing complete. Results written to:", output_file)