//
//  ViewController.swift
//  Draw and Play Video Game
//
//  Created by user183570 on 11/11/20.
//

import UIKit

let demoData:[MapObject] = [
    ("o",CGPoint(x:1.0,y:1.0),90.0,48.0),
    ("b",CGPoint(x:-60.0,y:-120.0),90.0,48.0),
    ("b",CGPoint(x:60.0,y:-240.0),90.0,48.0),
    ("x",CGPoint(x:10.0,y:200.0),90.0,48.0),
    ("w",CGPoint(x:20.0,y:390.0),0.0,314.0),
    ("w",CGPoint(x:-130.0,y:233.0),-90.0,314.0),
    ("w",CGPoint(x:7.0,y:76.0),0.0,233.0),
    ("w",CGPoint(x:125.0,y:200.0),90.0,233.0),
    ("w",CGPoint(x:25.0,y:301.0),-180.0,176.0),
    ("w",CGPoint(x:-56.0,y:230.0),-270.0,100.0)
]

class ViewController: UIViewController, UIImagePickerControllerDelegate, UINavigationControllerDelegate {
    var gameData:[MapObject]?
    @IBOutlet weak var startButton: UIButton!
    
    @IBAction func takePhoto(_ sender: Any) {
        let actionSheetController = UIAlertController(title: "Choose your image", message: nil, preferredStyle: .actionSheet)
        
        let cancelAction = UIAlertAction(title: "Cancel", style: UIAlertAction.Style.cancel) { (alertAction) -> Void in
            //do nothing
        }
        
        let takingPicturesAction = UIAlertAction(title: "Take Photo(unrecommended)", style: UIAlertAction.Style.default) { (alertAction) -> Void in
            self.getImageGo(type: 1)
        }
        
        let photoAlbumAction = UIAlertAction(title: "Album", style: UIAlertAction.Style.default) { (alertAction) -> Void in
            self.getImageGo(type: 2)
        }
                
        actionSheetController.addAction(cancelAction)
        actionSheetController.addAction(takingPicturesAction)
        actionSheetController.addAction(photoAlbumAction)
        
        //Set anchor point for iPad device floating layer
        actionSheetController.popoverPresentationController?.sourceView = sender as? UIView
        //show
        self.present(actionSheetController, animated: true, completion: nil)
        
    }
    
    @IBOutlet weak var imageView: UIImageView!
    var takingPicture:UIImagePickerController!
    
    override func viewDidLoad() {	
        super.viewDidLoad()
        
        // Do any additional setup after loading the view.
        
        
        startButton.isEnabled = false
        
//        let button = UIButton.init(type: .custom)
//
//                view.addSubview(button)
//                button.backgroundColor = UIColor.red
//                button.frame = CGRect.init(x: 100, y: 100, width: 200, height: 30)
//                button.tag = 1
//                button.addTarget(self, action: #selector(tapped), for: .touchUpInside)
    }
    
    
    func getImageGo(type:Int){
        takingPicture =  UIImagePickerController.init()
        if(type==1){
            takingPicture.sourceType = .camera
            //Whether to show the toolbar when taking pictures
            //takingPicture.showsCameraControls = true
        }else if(type==2){
            takingPicture.sourceType = .photoLibrary
        }
        //Whether to intercept, set to true to intercept the picture into a square after obtaining it
        takingPicture.allowsEditing = false
        takingPicture.delegate = self
        present(takingPicture, animated: true, completion: nil)
    }
    
    //Take a photo or choose the returned picture from the album
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        takingPicture.dismiss(animated: true, completion: nil)
        if(takingPicture.allowsEditing == false){
            //Original image
            let resultobjs = LineDetectorBridge().image2map(info[UIImagePickerController.InfoKey.originalImage]as? UIImage)
            let resultImage = LineDetectorBridge().detectLine(info[UIImagePickerController.InfoKey.originalImage]as? UIImage)
            
            let dict = convertToDictionary(text: resultobjs!)
            print("This is the result: ")
            gameData = convertToMapobj(dict: dict!)
            imageView.image = resultImage
        }else{
            //Screenshot; Unprocessed
            imageView.image = info[UIImagePickerController.InfoKey.editedImage] as? UIImage
        }
        startButton.isEnabled = true
 
    }
    
    func convertToMapobj(dict: [String: Any]) -> [MapObject] {
        let objs = dict["gameobjs"] as? [[String: Any]]
        var data:[MapObject] = []
        for obj in objs!{
            let type:String = obj["type"] as! String
            let x:CGFloat = obj["x"] as! CGFloat
            let y:CGFloat = obj["y"] as! CGFloat
            let angle:CGFloat = obj["angle"] as! CGFloat
            let size:CGFloat = obj["size"] as! CGFloat
            let gameobj:MapObject  = (
                type,CGPoint(x:CGFloat(x),y:CGFloat(y)),CGFloat(angle),CGFloat(size))
            data.append(gameobj)
        }
        print(data)
        return data
    }
    
    func convertToDictionary(text: String) -> [String: Any]? {
        if let data = text.data(using: .utf8) {
            do {
                return try JSONSerialization.jsonObject(with: data, options: []) as? [String: Any]
            } catch {
                print(error.localizedDescription)
            }
        }
        return nil
    }
    
    @IBAction func showDemo(_ sender: Any) {
        gameData = demoData
    }
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        super.prepare(for: segue, sender: sender)
        if let vc = segue.destination as? GameViewController {
            
            vc.gameData = gameData ?? demoData
        }
    }
}
