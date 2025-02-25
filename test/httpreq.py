import requests

url = "http://localhost:80/json" 
response = requests.get(url)
print(response.json())

