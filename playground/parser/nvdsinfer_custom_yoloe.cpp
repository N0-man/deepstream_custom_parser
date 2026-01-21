#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>

#include "nvdsinfer_custom_impl.h"

static float iou_xyxy(const NvDsInferObjectDetectionInfo& a,
                      const NvDsInferObjectDetectionInfo& b) {
  float x1 = std::max(a.left, b.left);
  float y1 = std::max(a.top,  b.top);
  float x2 = std::min(a.left + a.width,  b.left + b.width);
  float y2 = std::min(a.top  + a.height, b.top  + b.height);
  float iw = std::max(0.0f, x2 - x1);
  float ih = std::max(0.0f, y2 - y1);
  float inter = iw * ih;
  float areaA = a.width * a.height;
  float areaB = b.width * b.height;
  return inter / (areaA + areaB - inter + 1e-9f);
}

// Very small NMS (class-aware)
static void nms(std::vector<NvDsInferObjectDetectionInfo>& dets, float iouThr) {
  std::sort(dets.begin(), dets.end(),
            [](auto& a, auto& b){ return a.detectionConfidence > b.detectionConfidence; });

  std::vector<NvDsInferObjectDetectionInfo> keep;
  keep.reserve(dets.size());

  for (auto& d : dets) {
    bool suppressed = false;
    for (auto& k : keep) {
      if (d.classId == k.classId && iou_xyxy(d, k) > iouThr) {
        suppressed = true;
        break;
      }
    }
    if (!suppressed) keep.push_back(d);
  }
  dets.swap(keep);
}

extern "C"
bool NvDsInferParseYoloE(
    std::vector<NvDsInferLayerInfo> const& outputLayersInfo,
    NvDsInferNetworkInfo const& networkInfo,
    NvDsInferParseDetectionParams const& detectionParams,
    std::vector<NvDsInferObjectDetectionInfo>& objectList)
{
  // Expect one output: output0 [1, 75, 8400] float
  if (outputLayersInfo.empty()) return false;
  auto const& out = outputLayersInfo[0];

  // Validate dims
  auto const& dims = out.inferDims; // NvDsInferDims
  // dims.numDims should be 3; dims.d = {1, 75, 8400}
  if (dims.numDims != 3 || dims.d[0] != 1) return false;
  int C = dims.d[1];       // 75
  int N = dims.d[2];       // 8400
  int numClasses = C - 4;  // 71

  // Thresholds from config (class-attrs-all) arrive via detectionParams
  // detectionParams.perClassThreshold[cls]
  float iouThr = 0.45f; // or read from your own config; simplest: hardcode and tune later

  const float* data = reinterpret_cast<const float*>(out.buffer);
  if (!data) return false;

  // Layout is (1, C, N). For each candidate j:
  // xywh = data[0*C*N + 0*N + j], data[1*N + j], data[2*N + j], data[3*N + j]
  // scores for class k at data[(4+k)*N + j]
  std::vector<NvDsInferObjectDetectionInfo> dets;
  dets.reserve(300);

  for (int j = 0; j < N; ++j) {
    float cx = data[0 * N + j];
    float cy = data[1 * N + j];
    float w  = data[2 * N + j];
    float h  = data[3 * N + j];

    // Find best class
    int bestCls = -1;
    float bestScore = 0.0f;
    for (int k = 0; k < numClasses; ++k) {
      float s = data[(4 + k) * N + j];
      if (s > bestScore) { bestScore = s; bestCls = k; }
    }
    if (bestCls < 0) continue;

    float thr = detectionParams.perClassThreshold[bestCls];
    if (bestScore < thr) continue;

    // xywh -> xyxy stored as left/top/width/height
    NvDsInferObjectDetectionInfo det;
    det.classId = bestCls;
    det.detectionConfidence = bestScore;

    float x1 = cx - w * 0.5f;
    float y1 = cy - h * 0.5f;

    // Clamp to network input dims
    x1 = std::max(0.0f, std::min(x1, (float)networkInfo.width  - 1));
    y1 = std::max(0.0f, std::min(y1, (float)networkInfo.height - 1));
    w  = std::max(0.0f, std::min(w,  (float)networkInfo.width));
    h  = std::max(0.0f, std::min(h,  (float)networkInfo.height));

    det.left = x1;
    det.top = y1;
    det.width = w;
    det.height = h;

    dets.push_back(det);
  }

  nms(dets, iouThr);
  objectList = std::move(dets);
  return true;
}

// Register symbol (optional; depending on build style)
CHECK_CUSTOM_PARSE_FUNC_PROTOTYPE(NvDsInferParseYoloE);
