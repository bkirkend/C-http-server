import requests

url = "http://localhost:5000/json" response = requests.get(url)
print(response.json())

