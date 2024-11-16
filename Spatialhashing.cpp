#include <bits/stdc++.h>
using namespace std;
std::random_device rd;
std::mt19937 gen(rd());
using GridCell = pair<int, int>;
struct Point{
    float x = 0, y = 0;
    Point(float px, float py): x(px), y(py){}
    Point () = default;
    GridCell currCell;
    bool operator==(const Point& other){
        return x == other.x && y == other.y;
    }
};
using Object = Point;
 
enum ErrorCode {
    Duplicate, 
    Invalid,
    NotFound,
    OutOfRange,
    No,
    Unknown,
    None  // No error
};
struct HashFunction{
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>()(pair.first);
        auto hash2 = std::hash<T2>()(pair.second);
        return hash1 ^ (hash2 << 1); // Combine hashes
    }
};
using SpatialMap = unordered_map<GridCell, vector<Object>, HashFunction>;
void logError(ErrorCode code, string type, const string& context = " ") {
    switch (code) {
        case ErrorCode::Duplicate:
            cerr << "Error: Duplicate " << type << " detected. " << context << '\n';
            break;
        case ErrorCode::Invalid:
            cerr << "Error: Invalid " << type << " detected. " << context << '\n';
            break;
        case ErrorCode::NotFound:
            cerr << "Error: " << type << " not found. " << context << '\n';
            break;
        case ErrorCode::OutOfRange:
            cerr << "Error: Index " << type << " is out of range. " << context << '\n';
            break;
        case ErrorCode::No:
            cerr << "Error: No " << type << " detected. " << context << '\n';
            break;
        case ErrorCode::None: 
            break;
        case ErrorCode::Unknown:
            cerr << "Error: Unknown" << '\n';
            break;
        default: 
            cerr << "Unknown Error code of type " << type << " " + context << '\n';
            break;
    }
}
    GridCell calculateGridCell(const float x, const float y, const float cellSize){
        int cellX = static_cast<int>(floor(x / cellSize));
        int cellY = static_cast<int>(floor(y / cellSize));
        return {cellX, cellY};
    }
    void addObjectToCell(SpatialMap& hashMap, const Object& obj, const float spacing ){
        GridCell cell = calculateGridCell(obj.x, obj.y, spacing);
        hashMap[cell].push_back(obj);
    }
    vector<Object> queryObjects(SpatialMap& hashMap,float x, float y, float range, float cellSize){
         
        int xMin = static_cast<int>(floor((x - range) / cellSize));
        int xMax = static_cast<int>(floor((x + range) / cellSize));
        int yMin = static_cast<int>(floor((y - range) / cellSize));
        int yMax = static_cast<int>(floor((y + range) / cellSize));
        vector<Object> result;
        for(int gx = xMin; gx <= xMax; gx++){
            for(int gy = yMin; gy <= yMax; gy++){
                GridCell cell = {gx, gy};
                auto it = hashMap.find(cell);
                if(it != hashMap.end()){
                    result.insert(result.begin(), it->second.begin(), it->second.end());
                }
            }
        }
        return result;
    }
    void removeObjectFromMap(SpatialMap& hashMap, Object& objToRemove){
        GridCell cell = objToRemove.currCell;
        
        auto it =  hashMap.find(cell);
        if(it != hashMap.end()){
            auto& objects = it->second;
            cout << "Erasing Point " << objToRemove.x << " " << objToRemove.y << '\n';
            objects.erase(remove(objects.begin(), objects.end(), objToRemove), objects.end());
            if(objects.empty()){
                hashMap.erase(it);
            }
        }
    }
    void moveObjectToDiffCell(SpatialMap& hashMap, Object& obj, float cellSize){
        removeObjectFromMap(hashMap, obj);
        addObjectToCell(hashMap, obj, cellSize);
        
    }
vector<Object> createPoints(int x, float constraintX, float constraintY){
    vector<Object> pointArr;
    for(int i = 0; i < x; i++){
        uniform_real_distribution<> dis1(constraintX);
        uniform_real_distribution<> dis2(constraintY);
        float randX = dis1(gen);
        float randY = dis2(gen);
        Object newObject = Object(randX, randY);
        pointArr.push_back(newObject);
    }
    return pointArr;
}
int main(){
    SpatialMap spatialMap;
    const float cellSize = 10.f;
    vector<Object> points = createPoints(100000, 100, 100);
    for(auto& point : points){
        addObjectToCell(spatialMap, point, cellSize);
    }
    auto neighbors = queryObjects(spatialMap, 10, 10, 5, cellSize);
    cout << neighbors.size() << '\n';
     
}
