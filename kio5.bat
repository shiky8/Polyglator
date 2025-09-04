
""":"
:;echo "Running linux Shell commands..."
:;ls -la  
:;whoami
:;exit 0

@ECHO OFF
ECHO Running Batch windows commands...
dir
whoami
goto :eof
":"""
import os, getpass
print("Running Python code...")
print("Current directory:", os.getcwd())
print("Files:", [f for f in os.listdir('.') if not f.startswith('.')])  
print("User:", getpass.getuser())

